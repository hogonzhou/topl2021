#include "explode_insert.h"
#include "basefunc.h"
#include "TCHAR.h"


void 
xb()
{
	//ը���鼰���п�.
    //AcDbVoidPtrArray ptrArray;
	bool bexstblk = false;//�ж��Ƿ���"��".
	ads_name ssAll,ssunit;

	acutPrintf(_T("\n��ѡ���ʵ��"));
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
		//��ѡ����ÿ��ʵ�崦��֮.
		acedSSName(ssAll,i,ssunit);
		acdbGetObjectId(id,ssunit);
		acdbOpenAcDbEntity(pent,id,AcDb::kForRead);//ȡ��ʵ��ID��.

		if(pent->isKindOf(AcDbMInsertBlock::desc()))
		{
			//������п�.
			bexstblk = true;
			pent->close(); 

			AcGeMatrix3d mat;
   		    //mat.setToScaling(1.0);
			AcDbMInsertBlock *pminsertblock;
			acdbOpenObject(pminsertblock,id,AcDb::kForRead);
			bool bflag = explodeMinsert(pminsertblock,mat);
			if(bflag == true)
			{
				acutPrintf(_T("\nը����һ�����п�..."));
				pminsertblock->upgradeOpen();
				pminsertblock->erase();//ɾ�����п�.
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
			//������ͨ��(�Ƕ��п�);
			bexstblk = true;
			pent->close();  
			AcDbBlockTableRecord *pblktblrcd;
			AcDbBlockTableRecordIterator* pitr;//���б���ָ��.
			AcDbObjectId objblkrcdId;
			AcDbBlockReference *pblkref;
			acdbOpenObject(pblkref,id,AcDb::kForRead);

			AcGeMatrix3d matrix3d,matrixtmp;
			matrix3d = pblkref->blockTransform();//��ȡ"��"��"�任".
			AcGeScale3d scale3d;
			scale3d.extractScale(matrix3d);
			if(!matrix3d.isUniScaledOrtho())
			{
				acutPrintf(_T("\n=>=>=>����***�˴�������������һ��,����..."));
				//acutPrintf("XYZ���������ֱ���%15f,%15f,%15f",
				//		   scale3d.operator [](0),
				//		   scale3d.operator [](1),
				//		   scale3d.operator [](2));
				pblkref->close();
				continue;
			}							
							
			objblkrcdId = pblkref->blockTableRecord();
			pblkref->upgradeOpen();
			pblkref->erase();//ɾ���ÿ�.
			pblkref->close();
			acdbOpenObject(pblktblrcd,objblkrcdId,AcDb::kForRead);
			pblktblrcd->newIterator(pitr);
			pblktblrcd->close();

			AcDbEntity *pnewent;
			AcDbEntity *poldent;
			for(pitr->start();!pitr->done();pitr->step())
			{
				//ע��:�ڱ���"����¼",��������ʵ����ɾ����?									
				pitr->getEntity(poldent,AcDb::kForRead);//��ȡ����ָ��ָ���ʵ��.
				if(poldent->isKindOf(AcDbMInsertBlock::desc()))
				{
					//������ʵ���Ƕ��п�.
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
				    acutPrintf(_T("\nը������."));
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
		acutPrintf(_T("\n=====   ͼ����ͷû�п���! =="));
		acutPrintf(_T("\n============================"));
	}
	return;
} 




bool
explodeMinsert(AcDbMInsertBlock *pminsert,AcGeMatrix3d matrix3d_parent)
{
	//ը�����п�.	  matrix3d_parent�Ƕ��п���ϲ���"����".
	// AcGeMatrix3d matrix3d_parent = pblkreference->blockTransform();
    acutPrintf(_T("\n�ڶ��п���ͷ!"));
	AcGeScale3d scale3d;

	double angle = pminsert->rotation();
    AcGeMatrix3d matrix3d_offspring;//ָ���п鱾���"����".
    matrix3d_offspring = pminsert->blockTransform();
    scale3d.extractScale(matrix3d_offspring);
    if(!matrix3d_offspring.isUniScaledOrtho())
    {
        acutPrintf(_T("\n=>=>=>����:�˶��п�����������һ��,����..."));
		pminsert->close();																		
		return false;
	}
	AcGeVector3d vec2move;//�ƶ�ʸ��.
	AcGeMatrix3d mat2move;//�����ƶ�ʸ���ľ���..
				
	double rotation = pminsert->rotation();//�����ת�Ƕ�. 
	int irow,icol;
	ads_real fstepx,fstepy;
	irow = pminsert->rows();
	icol = pminsert->columns();
   	fstepx = pminsert->columnSpacing();
	fstepy = pminsert->rowSpacing();
	acutPrintf(_T("�ܹ���%d��%d��,�о�%f�о�%f."),irow,icol,fstepy,fstepx);

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
				vec2move.rotateBy(angle,AcGeVector3d::kZAxis);//��Z��Ϊ��׼��ת,�����ַ���.
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