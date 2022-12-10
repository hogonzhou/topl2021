#include "cl04_align_qylogo_date.h"
#include "basefunc.h"
#include "ext.h"
#include "qylogo.h"
#include "TCHAR.h"

void 
cl04_align_qylogo_date(const ACHAR *filename)
{
	//for cl04 custom specially.
	if(wcscmp(filename,_T("CL04")) == 0||wcscmp(filename,_T("cl04")) == 0
		||wcscmp(filename,_T("Cl04")) == 0||wcscmp(filename,_T("cL04")) == 0)
	{	
		ads_point base_pt;
		base_pt[0] = 68.24096960;
        base_pt[1] = -98.89556580; 		 
        double scale_logo = 1.8003;
		draw_logo(base_pt,scale_logo);//画LOGO;

		AcGePoint2d pt;
		pt.set(66.8457,-100.7692);
		add_nail(pt,23.0942,5.6772);//画一个长方型;
		return;
	}
}
