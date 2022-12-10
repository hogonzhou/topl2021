/*----------------------------------------------------------------------
*copyright:
*company:
*author:
*function:
*date:
----------------------------------------------------------------------*/
#ifndef _ZHOUHUAGANG_OWAL_SHORTCIRCUIT_H_
#define _ZHOUHUAGANG_OWAL_SHORTCIRCUIT_H_

/*
* include file
*/
#include <dbpl.h>
#include <adesk.h>


/*
*  function definition.
*/
void hello();


/*
*   const variable definition.
*/
#define toler 0.0000001





/*
*  polyline segment description;
*/
struct
SegmentDesc
{
};




/*
*  enumrate the relation between two segments.
*/
enum
RELAT_BTW_TWO_SEGS
{
	NULL_RELAT,	//just for the null relation.
	
	PNT_MIDDLE_END,		//the point on the middle of the main segment,and on the end of the primary
	PNT_HEAD_END,		//the head is identical to the end.
	PNT_END_END,		//the end of the main is identical to the end of the primary.
	PNT_HEAD_MIDDLE,
	PNT_END_MIDDLE,
	PNT_END_HEAD,
	PNT_HEAD_HEAD,
	PNT_MIDDLE_HEAD,
	PNT_MIDDLE_MIDDLE,	//real short circuit point.

	IDENT_OVER_HEAD_LESS_END,//IDENT mean the same direction.
	IDENT_EQUAL_HEAD_LESS_END,
	IDENT_LESS_HEAD_LESS_END,

	IDENT_OVER_HEAD_EQUAL_END,
	IDENT_EQUAL,
	IDENT_LESS_HEAD_EQUAL_END,

	IDENT_OVER_HEAD_OVER_END,
	IDENT_EQUAL_HEAD_OVER_END,
	IDENT_LESS_HEAD_OVER_HEAD,

	REVER_LESS_HEAD_OVER_END,//REVER  mean the same direction.
	REVER_LESS_HEAD_EUQAL_END,
	REVER_LESS_HEAD_LESS_END,

	REVER_EQUAL_HEAD_OVER_END,
	REVER_EQUAL,
	REVER_EUQAL_HEAD_LESS_END,

	REVER_OVER_HEAD_OVER_END,
	REVER_OVER_HEAD_EUQAL_END,
	REVER_OVER_HEAD_LESS_END
};




/*
*  description of the segment pair,i.e,the short-circuit point.
*/
struct
SEGMENT_PAIR
{
};




/*
*definition of class short-circuit,used to find out if the polyline is short-circuit,
*and to repair the polyline.
*/
class 
CShortCircuit
{
};




#endif //_ZHOUHUAGANG_OWAL_SHORTCIRCUIT_H_




