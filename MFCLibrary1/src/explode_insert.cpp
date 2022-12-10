#include "explode_insert.h"
#include "basefunc.h"
#include "TCHAR.h"


void 
xb()
{
	//炸开块及多行块.
    //AcDbVoidPtrArray ptrArray;
	bool bexstblk = false;//判断是否有"块".
	ads_name ssAll,ssunit;

	acutPrintf(_T("\n请选择块实体"));
	if(select(ssAll) != true)
	{
		return;
	}
	
	Adesk::Int32 lenth;
    acedSSLength(ssAll,&lenth);
             
    AcDbEntity *pent;
    AcDbObjectId id;			  			  
    for(long i = 0; i < lenth; i++)
	{
		//对选择集中每个实体处理之.
		acedSSName(ssAll,i,ssunit);
		acdbGetObjectId(id,ssunit);
		acdbOpenAcDbEntity(pent,id,AcDb::kForRead);//取得实体ID号.

		if(pent->isKindOf(AcDbMInsertBlock::desc()))
		{
			//处理多行块.
			bexstblk = true;
			pent->close(); 

			AcGeMatrix3d mat;
   		    //mat.setToScaling(1.0);
			AcDbMInsertBlock *pminsertblock;
			acdbOpenObject(pminsertblock,id,AcDb::kForRead);
			bool bflag = explodeMinsert(pminsertblock,mat);
			if(bflag == true)
			{
				acutPrintf(_T("\n炸开了一个多行块..."));
				pminsertblock->upgradeOpen();
				pminsertblock->erase();//删除多行块.
				pminsertblock->close(); 										
			}
			else
			{
				pminsertblock->close();
			}
			continue;
		}
		else if(pent->isKindOf(AcDbBlockReference::desc()))
		{
			//处理普通块(非多行块);
			bexstblk = true;
			pent->close();  
			AcDbBlockTableRecord *pblktblrcd;
			AcDbBlockTableRecordIterator* pitr;//块中遍历指针.
			AcDbObjectId objblkrcdId;
			AcDbBlockReference *pblkref;
			acdbOpenObject(pblkref,id,AcDb::kForRead);

			AcGeMatrix3d matrix3d,matrixtmp;
			matrix3d = pblkref->blockTransform();//获取"块"的"变换".
			AcGeScale3d scale3d;
			scale3d.extractScale(matrix3d);
			if(!matrix3d.isUniScaledOrtho())
			{
				acutPrintf(_T("\n=>=>=>警告***此处块收缩比例不一致,跳过..."));
				//acutPrintf("XYZ收缩比例分别是%15f,%15f,%15f",
				//		   scale3d.operator [](0),
				//		   scale3d.operator [](1),
				//		   scale3d.operator [](2));
				pblkref->close();
				continue;
			}							
							
			objblkrcdId = pblkref->blockTableRecord();
			pblkref->upgradeOpen();
			pblkref->erase();//删除该块.
			pblkref->close();
			acdbOpenObject(pblktblrcd,objblkrcdId,AcDb::kForRead);
			pblktblrcd->newIterator(pitr);
			pblktblrcd->close();

			AcDbEntity *pnewent;
			AcDbEntity *poldent;
			for(pitr->start();!pitr->done();pitr->step())
			{
				//注意:在遍历"块表记录",遍历到的实体能删除吗?									
				pitr->getEntity(poldent,AcDb::kForRead);//获取遍历指针指向的实体.
				if(poldent->isKindOf(AcDbMInsertBlock::desc()))
				{
					//当块中实体是多行块.
					AcDbMInsertBlock* pminsertblock;
					acdbOpenObject(pminsertblock,poldent->objectId(),AcDb::kForRead);
					poldent->close();
					bool bflag = explodeMinsert(pminsertblock,matrix3d);
					//pminsertblock->upgradeOpen();
					//pminsertblock->erase();
					pminsertblock->close();
				}						
				else
				{
				    poldent->upgradeOpen();
				    if(poldent->getTransformedCopy(matrix3d,pnewent) != Acad::eOk)
				    acutPrintf(_T("\n炸开错误."));
				    pnewent->upgradeOpen();
				    //pnewent->upgradeOpen();
				    join2database(pnewent);
				    pnewent->close();
				    poldent->close();
				}                   									    
			}							
		}	
		else
		{
			pent->close();						 
		}
	}
	acedSSFree(ssAll);
	if(!bexstblk)
	{
		acutPrintf(_T("\n============================"));
		acutPrintf(_T("\n=====   图形里头没有块了! =="));
		acutPrintf(_T("\n============================"));
	}
	return;
} 




bool
explodeMinsert(AcDbMInsertBlock *pminsert,AcGeMatrix3d matrix3d_parent)
{
	//炸开多行块.	  matrix3d_parent是多行块的上层块的"矩阵".
	// AcGeMatrix3d matrix3d_parent = pblkreference->blockTransform();
    acutPrintf(_T("\n在多行块里头!"));
	AcGeScale3d scale3d;

	double angle = pminsert->rotation();
    AcGeMatrix3d matrix3d_offspring;//指多行块本身的"矩阵".
    matrix3d_offspring = pminsert->blockTransform();
    scale3d.extractScale(matrix3d_offspring);
    if(!matrix3d_offspring.isUniScaledOrtho())
    {
        acutPrintf(_T("\n=>=>=>警告:此多行块收缩比例不一致,跳过..."));
		pminsert->close();																		
		return false;
	}
	AcGeVector3d vec2move;//移动矢量.
	AcGeMatrix3d mat2move;//定义移动矢量的矩阵..
				
	double rotation = pminsert->rotation();//块的旋转角度. 
	int irow,icol;
	ads_real fstepx,fstepy;
	irow = pminsert->rows();
	icol = pminsert->columns();
   	fstepx = pminsert->columnSpacing();
	fstepy = pminsert->rowSpacing();
	acutPrintf(_T("总共是%d行%d列,行距%f列距%f."),irow,icol,fstepy,fstepx);

	AcDbEntity* pnewent;
    pminsert->upgradeOpen();
	int i = 0,j = 0;
    // for(int i = 0;i <= irow; i++)
	do
{
          //for(int j = 0; j <= icol; j++)
		  do
		  {
			    vec2move.set(j*fstepx,i*fstepy,0);
				vec2move.rotateBy(angle,AcGeVector3d::kZAxis);//以Z轴为基准旋转,按右手法则.
				vec2move.transformBy(matrix3d_parent);
				mat2move.setTranslation(vec2move);						
				//mat2move.setToProduct(mat2move,matrix3d_parent);
				//mat2move.setToProduct(mat2move,matrix3d_offspring);						
				if(pminsert->getTransformedCopy(matrix3d_parent,pnewent) != Acad::eOk)
				{
					pnewent->close();
					pminsert->close();
					return false;
				}
				pnewent->transformBy(mat2move);
				join2database(pnewent);
				pnewent->close();
				j++;
		  }while(j < icol);
		  i++;
		  j = 0;
	}while(i < irow);
		//pminsert->erase();
		//pminsert->close();
	return true;
}