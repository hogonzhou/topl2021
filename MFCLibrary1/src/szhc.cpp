#include "szhc.h"
#include "basefunc.h"

#include "TCHAR.h"


	double inRad,outRad;//��,�⾶.

void suzhouhongce()
{//���ļ�"szhc.txt"��ȡ������������,��Բ.���ݺ��ר��.
	int flag1,flag2;
	//double inRad,outRad;//��,�⾶.
	do{
		flag1 = acedGetReal(_T("\n������Բ���⾶(OD):"),&outRad);
		if(flag1 == RTCAN)return;
		flag2 = acedGetReal(_T("\n������Բ���ھ�(ID):"),&inRad);
		if(flag2 == RTCAN)return;
	}while(flag1 != RTNORM || flag2 != RTNORM);
	
	char chArr[8192][16];
	int i = 0,j = 0;//����ֵ.
	bool flag3 = getFileData(chArr,i);
	if(!flag3)
	{
		acutPrintf(_T("\n��ȡ���ݳ���,����szhc.txt�ļ���֪ͨ����ʦ!"));
		return;
	}
	outPutString(chArr,i);

	//bool flag4 = chkFileData(chArr,i);//��chArr[][]���м��...
	//if(!flag4)
	//	{
	//		acutPrintf("\n��������,��������Ϣ,�˳�...");
	//		return;
	//	}

	/*
	double x,y;
	acutPrintf("\n i = %d,j = %d",i,j);
	for(int t = 0; t < i;t++)
	{
		//acutPrintf("\n%s",chArr[t]);//�������.
		x = atof(chArr[t]);
		y = atof(chArr[t+1]);
		addcircle(x,y,inRad/2.0);
		addcircle(x,y,outRad/2.0);
		t++;
	}	
	*/
}
//1-��ȡ����,�������;2-������ͳ��ȴ洢����;3-�������黭Բ��.
//1-1 




void addcircle(double x,double y,double radius)
{	
	addLayer(_T("szhc_circles"));
	AcDbCircle *circle = new AcDbCircle();
	AcGePoint3d ptCenter;
	AcGeVector3d vec3d(0,0,1);
	ptCenter.set(x,y,0);//���ĵ�.

	circle->setCenter(ptCenter);
	circle->setRadius(radius);
	circle->setNormal(vec3d);
	circle->setLayer(_T("szhc_circles"));

	join2database(circle);

	circle->close();
}




bool getFileData(char chArr[][16],int& i)
{//��ȡ�ļ�,�����ַ����鼰����������.
    int j = 0;
	FILE* fp;
	if((fp = fopen("d:/data/szhc.txt","r")) == NULL)
	{
		acutPrintf(_T("====>����:�ļ��򿪴���,��ֹ����,����!\n"));
		acutPrintf(_T("====>����:�ļ��򿪴���,��ֹ����,����!\n"));
		acutPrintf(_T("====>����:�ļ��򿪴���,��ֹ����,����!\n"));
		return false;
	}

//	char chArr[600][10];//��Ŵ��ļ���ȡ��ÿһ�������ַ��Ŵ�.���300�Ե�����.
//	int i = 0,j = 0,count=0;//i-��������,j-��������,count-�ල�����Ƿ����(������).
	char nChar;//��ÿһ��ȡ�õ��ַ��Ž����ж�.
	while((nChar=fgetc(fp)) != EOF)
	{
		if(nChar == '-')
		{
			if(j != 0)
			{
				acutPrintf(_T("\n%d�������ļ���ʽ��д����!"),i);
				i++;j = 0;
				chArr[i][j] = nChar;//-�ű��������鿪ͷ.
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
				acutPrintf(_T("\n===>����:����λ���ȳ�����16,�˳�!"));
				return false;
		  	}
			if(j == 0)
			{
				acutPrintf(_T("\n .�Ų����ڿ�ͷ"));
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
		          acutPrintf(_T("\n===>����:����λ���ȳ�����16,�˳�!"));
			      return false;
		      }
		  chArr[i][j] = nChar;
		  j++;
		}
		else
		{//nChar����-,��,���ֵ�ʱ��.
		   //if(i == 0 && j == 0)continue;//�����һ���ַ����Ծ�,�����.
			  if(j == 0)continue;//�����һ���ַ����Ծ�,�����ѭ��.
			  chArr[i][j] = '\0';//������.
			  i++;
			  j = 0;

			  if(i >= 8192)
			  {
	              double x,y;
	              acutPrintf(_T("\n i = %d,j = %d"),i,j);
	              for(int t = 0; t < i;t++)
	              {
		               //acutPrintf("\n%s",chArr[t]);//�������.
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
		//if(!isdigit(nChar) && (strcmp(&nChar,".")!=0))acutPrintf("\n����!");
		if(isdigit(nChar))acutPrintf("����!");
		else if(nChar == '.')acutPrintf("��!");
		else if(strcmp(&nChar,"") == 0)acutPrintf("�ո�A");
		else if(nChar == '\t')acutPrintf("�Ʊ��");
		else if(nChar == '\0')acutPrintf("�ո��B");
		else if(nChar == '\n')acutPrintf("�س���");
		else acutPrintf("����!");
		
		//return;   */
	}//end while();


   {
       double x,y;
       acutPrintf(_T("\n i = %d,j = %d"),i,j);
       for(int t = 0; t < i;t++)
       {
          //acutPrintf("\n%s",chArr[t]);//�������.
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
{//��chaArr[][]���ݽ��м��.
	int count1 = 0,count2 = 0,count3 =0;//��'-''.'����;
	bool flag = false;//ָʾ�Ƿ�һ'-'��ͷ,�����ж�'.'��λ���Ƿ���ȷ.
	if(i%2 != 0 || i == 0)//i��Ϊż���������
	{
		acutPrintf(_T("\n������Ϊ���Ϊż��,�˳�!"));
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
					count1++;//ͳ��'-'�Ÿ���.
					if(n != 0 || n == j)
					{
						acutPrintf(_T("\n n = %d,j=%d"),n,j);
						acutPrintf(_T("\n%d��-��λ�ò���,�����ݴ���!�˳�..."),m);
						return false;
					}
					flag = true;
					break;
				case '.':
					count2++;//ͳ��'.'�Ÿ���.
					if(n == 0 || n == j)
					{
						acutPrintf(_T("\n.�Ų�Ӧ�����ֿ�ͷ���β,�����ݴ���!�˳�..."));
						return false;
					}
					//	if(flag && n == 2)
					//	{
					//		acutPrintf("\n.��λ�ô���,�˳�...");
					//			return false;
				    //	}
					break;
				case '\0':
				    if(n != j)
					{
						acutPrintf(_T("\n�������ִ�,�˳�..."));
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
		}//����ѭ��
		if(count1 > 1 || count2 >1)
		{
			acutPrintf(_T("\n������ͷ�ж��.��-,����,�˳�...."));
			return false;
		}
		count1 = 0; count2 = 0;//�ָ�����.
	}//����ѭ��.
	return true;
}




void
outPutString(char chArr[][16],int& i)
{			
	//i -= (i%2);//��֤i��ż��.
	acutPrintf(_T("\n����%d������."),i);
	for(int count = 0; count < i/2; count += 2)
	{
		acutPrintf(_T("\n�ַ���%d��%s  %s"),count,chArr[count],chArr[count+1]);
	}
	return ;			
}



bool getFileDataNew(char chArr[][16],int& i)
{//��ȡ�ļ�,�����ַ����鼰����������.
    int j = 0;
	FILE* fp;
	if((fp = fopen("d:/data/szhc.txt","r")) == NULL)
	{
		acutPrintf(_T("====>����:�ļ��򿪴���,��ֹ����,����!\n"));
		acutPrintf(_T("====>����:�ļ��򿪴���,��ֹ����,����!\n"));
		acutPrintf(_T("====>����:�ļ��򿪴���,��ֹ����,����!\n"));
		return false;
	}

//	char chArr[600][10];//��Ŵ��ļ���ȡ��ÿһ�������ַ��Ŵ�.���300�Ե�����.
//	int i = 0,j = 0,count=0;//i-��������,j-��������,count-�ල�����Ƿ����(������).
	char nChar;//��ÿһ��ȡ�õ��ַ��Ž����ж�.
	while((nChar=fgetc(fp)) != EOF)
	{
		if(nChar == '-')
		{
			if(j != 0)
			{
				acutPrintf(_T("\n%d�������ļ���ʽ��д����!"),i);
				i++;j = 0;
				chArr[i][j] = nChar;//-�ű��������鿪ͷ.
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
				acutPrintf(_T("\n===>����:����λ���ȳ�����16,�˳�!"));
				return false;
		  	}
			if(j == 0)
			{
				acutPrintf(_T("\n .�Ų����ڿ�ͷ"));
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
		          acutPrintf(_T("\n===>����:����λ���ȳ�����16,�˳�!"));
			      return false;
		      }
		  chArr[i][j] = nChar;
		  j++;
		}
		else
		{//nChar����-,��,���ֵ�ʱ��.
		   //if(i == 0 && j == 0)continue;//�����һ���ַ����Ծ�,�����.
			  if(j == 0)continue;//�����һ���ַ����Ծ�,�����ѭ��.

			  chArr[i][j] = '\0';//������.
			  i++;
			  j = 0;

			  if(i > 8191)
			  {
			      acutPrintf(_T("\n===>����:���ֹ���,���޸ĳ��������ɸ�������!"));
				  fclose(fp);//�ر��ļ�.
				  return false;
			  }

		}
	  /*if(nChar == EOF)
			break;
		if(strcmp(&nChar,"") != 0)acutPrintf("%c",nChar);
		//if(!isdigit(nChar) && (strcmp(&nChar,".")!=0))acutPrintf("\n����!");
		if(isdigit(nChar))acutPrintf("����!");
		else if(nChar == '.')acutPrintf("��!");
		else if(strcmp(&nChar,"") == 0)acutPrintf("�ո�A");
		else if(nChar == '\t')acutPrintf("�Ʊ��");
		else if(nChar == '\0')acutPrintf("�ո��B");
		else if(nChar == '\n')acutPrintf("�س���");
		else acutPrintf("����!");
		
		//return;   */
	}//end while();
	fclose(fp);	
	return true;
}