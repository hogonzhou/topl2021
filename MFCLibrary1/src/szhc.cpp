#include "szhc.h"
#include "basefunc.h"

#include "TCHAR.h"


	double inRad,outRad;//内,外径.

void suzhouhongce()
{//将文件"szhc.txt"读取其中坐标数据,画圆.苏州鸿测专用.
	int flag1,flag2;
	//double inRad,outRad;//内,外径.
	do{
		flag1 = acedGetReal(_T("\n请输入圆的外径(OD):"),&outRad);
		if(flag1 == RTCAN)return;
		flag2 = acedGetReal(_T("\n请输入圆的内径(ID):"),&inRad);
		if(flag2 == RTCAN)return;
	}while(flag1 != RTNORM || flag2 != RTNORM);
	
	char chArr[8192][16];
	int i = 0,j = 0;//赋初值.
	bool flag3 = getFileData(chArr,i);
	if(!flag3)
	{
		acutPrintf(_T("\n读取数据出错,请检查szhc.txt文件或通知工程师!"));
		return;
	}
	outPutString(chArr,i);

	//bool flag4 = chkFileData(chArr,i);//对chArr[][]进行检查...
	//if(!flag4)
	//	{
	//		acutPrintf("\n数据有误,见报告信息,退出...");
	//		return;
	//	}

	/*
	double x,y;
	acutPrintf("\n i = %d,j = %d",i,j);
	for(int t = 0; t < i;t++)
	{
		//acutPrintf("\n%s",chArr[t]);//测试语句.
		x = atof(chArr[t]);
		y = atof(chArr[t+1]);
		addcircle(x,y,inRad/2.0);
		addcircle(x,y,outRad/2.0);
		t++;
	}	
	*/
}
//1-读取数据,获得坐标;2-用数组和长度存储数据;3-根据数组画圆形.
//1-1 




void addcircle(double x,double y,double radius)
{	
	addLayer(_T("szhc_circles"));
	AcDbCircle *circle = new AcDbCircle();
	AcGePoint3d ptCenter;
	AcGeVector3d vec3d(0,0,1);
	ptCenter.set(x,y,0);//中心点.

	circle->setCenter(ptCenter);
	circle->setRadius(radius);
	circle->setNormal(vec3d);
	circle->setLayer(_T("szhc_circles"));

	join2database(circle);

	circle->close();
}




bool getFileData(char chArr[][16],int& i)
{//读取文件,返回字符数组及数组行列数.
    int j = 0;
	FILE* fp;
	if((fp = fopen("d:/data/szhc.txt","r")) == NULL)
	{
		acutPrintf(_T("====>警告:文件打开错误,中止运行,请检查!\n"));
		acutPrintf(_T("====>警告:文件打开错误,中止运行,请检查!\n"));
		acutPrintf(_T("====>警告:文件打开错误,中止运行,请检查!\n"));
		return false;
	}

//	char chArr[600][10];//存放从文件读取的每一段连续字符号串.最多300对点坐标.
//	int i = 0,j = 0,count=0;//i-数组行数,j-数组列数,count-监督列数是否溢出(即报错).
	char nChar;//对每一个取得的字符号进行判断.
	while((nChar=fgetc(fp)) != EOF)
	{
		if(nChar == '-')
		{
			if(j != 0)
			{
				acutPrintf(_T("\n%d行数据文件格式书写有误!"),i);
				i++;j = 0;
				chArr[i][j] = nChar;//-号必须在数组开头.
				j++;
				continue;
			}
			chArr[i][j] = nChar;
			j++;
		}
		else if(nChar == '.')
		{
			if(j >= 16)
			{
				acutPrintf(_T("\n===>警告:数字位长度超过了16,退出!"));
				return false;
		  	}
			if(j == 0)
			{
				acutPrintf(_T("\n .号不能在开头"));
				i++;
				j = 0;
				continue;
			}
				chArr[i][j] = nChar;
				j++;
		  }
	      else if(isdigit(nChar))
		  {
		      if(j >= 16)
		      {
		          acutPrintf(_T("\n===>警告:数字位长度超过了16,退出!"));
			      return false;
		      }
		  chArr[i][j] = nChar;
		  j++;
		}
		else
		{//nChar不是-,点,数字的时候.
		   //if(i == 0 && j == 0)continue;//如果第一个字符不对劲,则继续.
			  if(j == 0)continue;//如果第一个字符不对劲,则继续循环.
			  chArr[i][j] = '\0';//结束符.
			  i++;
			  j = 0;

			  if(i >= 8192)
			  {
	              double x,y;
	              acutPrintf(_T("\n i = %d,j = %d"),i,j);
	              for(int t = 0; t < i;t++)
	              {
		               //acutPrintf("\n%s",chArr[t]);//测试语句.
		               x = atof(chArr[t]);
		               y = atof(chArr[t+1]);
		               //addcircle(x,y,inRad/2.0);
		               addcircle(x,y,outRad/2.0);
		               t++;
	              }	
				  i = 0;
			  }
		}
	  /*if(nChar == EOF)
			break;
		if(strcmp(&nChar,"") != 0)acutPrintf("%c",nChar);
		//if(!isdigit(nChar) && (strcmp(&nChar,".")!=0))acutPrintf("\n换行!");
		if(isdigit(nChar))acutPrintf("数字!");
		else if(nChar == '.')acutPrintf("点!");
		else if(strcmp(&nChar,"") == 0)acutPrintf("空格A");
		else if(nChar == '\t')acutPrintf("制表键");
		else if(nChar == '\0')acutPrintf("空格键B");
		else if(nChar == '\n')acutPrintf("回车键");
		else acutPrintf("其他!");
		
		//return;   */
	}//end while();


   {
       double x,y;
       acutPrintf(_T("\n i = %d,j = %d"),i,j);
       for(int t = 0; t < i;t++)
       {
          //acutPrintf("\n%s",chArr[t]);//测试语句.
          x = atof(chArr[t]);
          y = atof(chArr[t+1]);
          //addcircle(x,y,inRad/2.0);
          addcircle(x,y,outRad/2.0);
          t++;
        }	
	}


	fclose(fp);	
	return true;
}

bool 
chkFileData(char chArr[][16],int& i)
{//对chaArr[][]数据进行检查.
	int count1 = 0,count2 = 0,count3 =0;//对'-''.'计数;
	bool flag = false;//指示是否一'-'开头,用来判断'.'的位置是否正确.
	if(i%2 != 0 || i == 0)//i不为偶数或等于零
	{
		acutPrintf(_T("\n坐标数为零或不为偶数,退出!"));
		return false;
	}
	for(int m = 0; m <= i; m++ )
	{
		int j = strlen(chArr[m]);
  		for(int n = 0; n <= j; n++)
	    {
	        switch(chArr[m][n])
			{
			    case '-':
					count1++;//统计'-'号个数.
					if(n != 0 || n == j)
					{
						acutPrintf(_T("\n n = %d,j=%d"),n,j);
						acutPrintf(_T("\n%d行-号位置不对,读数据错误!退出..."),m);
						return false;
					}
					flag = true;
					break;
				case '.':
					count2++;//统计'.'号个数.
					if(n == 0 || n == j)
					{
						acutPrintf(_T("\n.号不应在数字开头或结尾,读数据错误!退出..."));
						return false;
					}
					//	if(flag && n == 2)
					//	{
					//		acutPrintf("\n.号位置错误,退出...");
					//			return false;
				    //	}
					break;
				case '\0':
				    if(n != j)
					{
						acutPrintf(_T("\n错误数字串,退出..."));
						return false;
					}
					break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				    break;
				default:
					break;
			}
		}//串内循环
		if(count1 > 1 || count2 >1)
		{
			acutPrintf(_T("\n数字里头有多个.或-,请检查,退出...."));
			return false;
		}
		count1 = 0; count2 = 0;//恢复变量.
	}//串间循环.
	return true;
}




void
outPutString(char chArr[][16],int& i)
{			
	//i -= (i%2);//保证i是偶数.
	acutPrintf(_T("\n共有%d行数据."),i);
	for(int count = 0; count < i/2; count += 2)
	{
		acutPrintf(_T("\n字符串%d是%s  %s"),count,chArr[count],chArr[count+1]);
	}
	return ;			
}



bool getFileDataNew(char chArr[][16],int& i)
{//读取文件,返回字符数组及数组行列数.
    int j = 0;
	FILE* fp;
	if((fp = fopen("d:/data/szhc.txt","r")) == NULL)
	{
		acutPrintf(_T("====>警告:文件打开错误,中止运行,请检查!\n"));
		acutPrintf(_T("====>警告:文件打开错误,中止运行,请检查!\n"));
		acutPrintf(_T("====>警告:文件打开错误,中止运行,请检查!\n"));
		return false;
	}

//	char chArr[600][10];//存放从文件读取的每一段连续字符号串.最多300对点坐标.
//	int i = 0,j = 0,count=0;//i-数组行数,j-数组列数,count-监督列数是否溢出(即报错).
	char nChar;//对每一个取得的字符号进行判断.
	while((nChar=fgetc(fp)) != EOF)
	{
		if(nChar == '-')
		{
			if(j != 0)
			{
				acutPrintf(_T("\n%d行数据文件格式书写有误!"),i);
				i++;j = 0;
				chArr[i][j] = nChar;//-号必须在数组开头.
				j++;
				continue;
			}
			chArr[i][j] = nChar;
			j++;
		}
		else if(nChar == '.')
		{
			if(j >= 16)
			{
				acutPrintf(_T("\n===>警告:数字位长度超过了16,退出!"));
				return false;
		  	}
			if(j == 0)
			{
				acutPrintf(_T("\n .号不能在开头"));
				i++;
				j = 0;
				continue;
			}
				chArr[i][j] = nChar;
				j++;
		  }
	      else if(isdigit(nChar))
		  {
		      if(j >= 16)
		      {
		          acutPrintf(_T("\n===>警告:数字位长度超过了16,退出!"));
			      return false;
		      }
		  chArr[i][j] = nChar;
		  j++;
		}
		else
		{//nChar不是-,点,数字的时候.
		   //if(i == 0 && j == 0)continue;//如果第一个字符不对劲,则继续.
			  if(j == 0)continue;//如果第一个字符不对劲,则继续循环.

			  chArr[i][j] = '\0';//结束符.
			  i++;
			  j = 0;

			  if(i > 8191)
			  {
			      acutPrintf(_T("\n===>警告:数字过多,请修改程序以容纳更多数据!"));
				  fclose(fp);//关闭文件.
				  return false;
			  }

		}
	  /*if(nChar == EOF)
			break;
		if(strcmp(&nChar,"") != 0)acutPrintf("%c",nChar);
		//if(!isdigit(nChar) && (strcmp(&nChar,".")!=0))acutPrintf("\n换行!");
		if(isdigit(nChar))acutPrintf("数字!");
		else if(nChar == '.')acutPrintf("点!");
		else if(strcmp(&nChar,"") == 0)acutPrintf("空格A");
		else if(nChar == '\t')acutPrintf("制表键");
		else if(nChar == '\0')acutPrintf("空格键B");
		else if(nChar == '\n')acutPrintf("回车键");
		else acutPrintf("其他!");
		
		//return;   */
	}//end while();
	fclose(fp);	
	return true;
}