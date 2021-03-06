/*
Legal:
	Version: MPL 1.1
	
	The contents of this file are subject to the Mozilla Public License Version 
	1.1 the "License"; you may not use this file except in compliance with 
	the License. You may obtain a copy of the License at 
	http://www.mozilla.org/MPL/
	
	Software distributed under the License is distributed on an "AS IS" basis,
	WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
	for the specific language governing rights and limitations under the
	License.
	
	The Original Code is the YSI framework.
	
	The Initial Developer of the Original Code is Alex "Y_Less" Cole.
	Portions created by the Initial Developer are Copyright C 2011
	the Initial Developer. All Rights Reserved.

Contributors:
	Y_Less
	koolk
	JoeBullet/Google63
	g_aSlice/Slice
	Misiur
	samphunter
	tianmeta
	maddinat0r
	spacemud
	Crayder
	Dayvison
	Ahmad45123
	Zeex
	irinel1996
	Yiin-
	Chaprnks
	Konstantinos
	Masterchen09
	Southclaws
	PatchwerkQWER
	m0k1
	paulommu
	udan111

Thanks:
	JoeBullet/Google63 - Handy arbitrary ASM jump code using SCTRL.
	ZeeX - Very productive conversations.
	koolk - IsPlayerinAreaEx code.
	TheAlpha - Danish translation.
	breadfish - German translation.
	Fireburn - Dutch translation.
	yom - French translation.
	50p - Polish translation.
	Zamaroht - Spanish translation.
	Los - Portuguese translation.
	Dracoblue, sintax, mabako, Xtreme, other coders - Producing other modes for
		me to strive to better.
	Pixels^ - Running XScripters where the idea was born.
	Matite - Pestering me to release it and using it.

Very special thanks to:
	Thiadmer - PAWN, whose limits continue to amaze me!
	Kye/Kalcor - SA:MP.
	SA:MP Team past, present and future - SA:MP.

Optional plugins:
	Gamer_Z - GPS.
	Incognito - Streamer.
	Me - sscanf2, fixes2, Whirlpool.
*/

#define Master_ID() (@_)

// These parts should only be defined once ever.  This code is not dependent on
// the current MASTER value, it's all generic.

#define _YSIM_COMPARE -1

#define MASTER_DATA<%0> static stock Bit:YSI_g_sMasterData[_:(%0)];
#define MASTER_RESET<%0> (YSI_g_sMasterData[_:(%0)] = Bit:0);
#define MASTER_SET<%0> (YSI_g_sMasterData[_:(%0)] = Bit:Master_Caller());
#define MASTER_ADD<%0> (YSI_g_sMasterData[_:(%0)] |= Bit:Master_Caller());
#define MASTER_REMOVE<%0> (YSI_g_sMasterData[_:(%0)] &= Bit:(~Master_Caller()));
#define MASTER_EMPTY<%0> if(!YSI_g_sMasterData[_:(%0)])
#define MASTER_EXCLUSIVE<%0> (YSI_g_sMasterData[_:(%0)] == Bit:Master_ID())
#define MASTER_GET<%0> (YSI_g_sMasterData[_:(%0)])
#define MASTER_COPY<%0,%1> (YSI_g_sMasterData[_:(%0)] = YSI_g_sMasterData[_:(%1)]);

#define _@MAKE_YCM<%0...%1> MAKE_YCM<_@%0...%1>
#define @@MAKE_YCM<%0...%1> MAKE_YCM<@@%0...%1>

//#define Master_State(%0); {  }

#if defined YSI_IS_CLIENT
	#if NO_VALUE(YSI_IS_CLIENT)
		#undef YSI_IS_CLIENT
		#define YSI_IS_CLIENT 100
	#endif
#endif

#if defined YSI_IS_SERVER
	#if NO_VALUE(YSI_IS_SERVER)
		#undef YSI_IS_SERVER
		#define YSI_IS_SERVER 100
	#endif
#endif

#if defined YSI_IS_STUB
	#if NO_VALUE(YSI_IS_STUB)
		#undef YSI_IS_STUB
		#define YSI_IS_STUB 100
	#endif
#endif

#if YSI_KEYWORD(mhook)
	 #define mhook MASTER_HOOK__
#endif
#if YSI_KEYWORD(mtask)
	 #define mtask MASTER_TASK__
#endif
#if YSI_KEYWORD(mptask)
	 #define mptask MASTER_PTASK__
#endif
#if YSI_KEYWORD(foreign)
	 #define foreign FOREIGN__
#endif
#if YSI_KEYWORD(global)
	 #define global GLOBAL__
#endif
#if YSI_KEYWORD(master_func)
	 #define master_func MASTER_FUNC__
#endif
#if YSI_KEYWORD(master_hook)
	 #define master_hook MASTER_HOOK__
#endif
#if YSI_KEYWORD(master_task)
	 #define master_task MASTER_TASK__
#endif
#if YSI_KEYWORD(master_ptask)
	 #define master_ptask MASTER_PTASK__
#endif
#if YSI_KEYWORD(group_hook)
	 #define group_hook GROUP_HOOK__
#endif

// Because "Cloud" and "Client" start with the same letter, "YSIMC" would be
// confusing.  "Cloud" and "Slave" would be just as confusing since "S" could be
// that or "Server" and "C" still wouldn't be disambiguated.  Also "Stub"
// complicates things further - why are they all the same letter?
// 
// Instead to avoid any confusion, we will use:
//   
//   D - Distributed (Cloud).
//   T - Thunk (Stub).
//   M - Master (Server, but was never ambiguous).
//   U - User (Client).
//   
// I want to make it clear that this internal rename is NOT for any sense of PC-
// ness around the term "slave" - it is a perfectly descriptive term for a node
// ordered to do work by a central controller (though inaccurate in this case as
// that is not what happens here - a central server is given work by many other
// clients).
#define YSIM_PARSER(%2)%0(%1) FUNC_PARSER(%2,NUM_CST_DEF:STR_CST_DEF:REF_DEF:ARR_CST:QAL:RET_VOD_STR_TAG:LEN:)(%0(%1))(%1)#()()()

#define YSIM_SERVER_FOREIGN        YSIM_PARSER(YSIM_MF)
// The global is just the function implementation - doesn't need extra code.
#define YSIM_SERVER_GLOBAL
#define YSIM_SERVER_HOOK%0(%1)     HOOK__%0(%1)
#define YSIM_SERVER_TASK%0[%3](%1) task%0[%3](%1)
#define YSIM_SERVER_PTSK%0[%3](%1) ptask%0[%3](%1)
#define YSIM_SERVER_FUNC%0(%1)<%9> forward%0(%1);public%0(%1)
#define YSIM_SERVER_GRUP%0(%1)     _GROUP_MAKE_NAME<HOOK__%0...>(%1)
#define YSIM_SERVER_YCMD%0(%1)     _YCMD_4:@yC_%0(a,b[],c);_YCMD_4:@yC_%0(a,b[],c)U@(8,YSIM_RETURN,@_yC%0(a,b,c));@_yC%0(%1);@_yC%0(%1)
#define YSIM_SERVER_ZERO%0(%1)     _YCMD_4:@yC_%0(a,b[],c,i);_YCMD_4:@yC_%0(a,b[],c,i)i==@_&&U@(8,YSIM_RETURN,@_yC%0(a,b,c));@_yC%0(%1);@_yC%0(%1)

#define YSIM_CLIENT_FOREIGN        YSIM_PARSER(YSIM_UF)
// Hide the local implementations.
#define YSIM_CLIENT_GLOBAL%0(%1)   YSIM_PARSER(YSIM_UG)%0(%1)
#define YSIM_CLIENT_HOOK%0(%1)     static stock _@%0(%1)
#define YSIM_CLIENT_TASK%0[%3](%1) YSIM_PARSER(YSIM_UG)%0(%1)
#define YSIM_CLIENT_PTSK%0[%3](%1) YSIM_PARSER(YSIM_UG)%0(%1)
#define YSIM_CLIENT_FUNC%0(%1)<%9> static stock %0(%1)
#define YSIM_CLIENT_GRUP%0(%1)     YSIM_PARSER(YSIM_UG)%0(%1)
#define YSIM_CLIENT_YCMD%0(%1)     static stock _YCMD_6:_@yC%0(%1)
#define YSIM_CLIENT_ZERO%0(%1)     YSIM_SERVER_ZERO%0(%1)

#define YSIM_CLOUD_FOREIGN         YSIM_PARSER(YSIM_DF)
#define YSIM_CLOUD_GLOBAL          YSIM_PARSER(YSIM_DG)
#define YSIM_CLOUD_HOOK%0(%1)      HOOK__%0(%1)<>return 0;HOOK__%0(%1)<_YCM:y>
#define YSIM_CLOUD_TASK%0[%3](%1)  task%0[%3](%1)<>{}public%0(%1)<_YCM:y>
#define YSIM_CLOUD_PTSK%0[%3](%1)  ptask%0[%3](%1)<>{}public%0(%1)<_YCM:y>
#define YSIM_CLOUD_FUNC%0(%1)<%9>  %0(%1);public%0(%1)<>{}public%0(%1)<%9>
#define YSIM_CLOUD_GRUP%0(%1)      _GROUP_MAKE_NAME<HOOK__%0...>(%1)<>{}_GROUP_MAKE_NAME<HOOK__%0...>(%1)<_YCM:y>
#define YSIM_CLOUD_YCMD%0(%1)      _YCMD_4:@yC_%0(a,b[],c);_YCMD_4:@yC_%0(a,b[],c)<>{}_YCMD_4:@yC_%0(a,b[],c)<_YCM:y>U@(8,YSIM_RETURN,@_yC%0(a,b,c));@_yC%0(%1);@_yC%0(%1)
#define YSIM_CLOUD_ZERO%0(%1)      _YCMD_4:@yC_%0(a,b[],c,i);_YCMD_4:@yC_%0(a,b[],c,i)i==@_&&U@(8,YSIM_RETURN,@_yC%0(a,b,c));@_yC%0(%1);@_yC%0(%1)

// A stub foreign is the same as a client foreign - define it as existing, but
// not here.
#define YSIM_STUB_FOREIGN          YSIM_PARSER(YSIM_UF)
// There is no stub global - that makes no sense (give an error).  Still needs
// parsing though to give the RIGHT error...
#define YSIM_STUB_GLOBAL%0(%1)     YSIM_PARSER(YSIM_TG)%0(%1)
#define YSIM_STUB_HOOK%0(%1)       YSIM_PARSER(YSIM_TG)%0(%1)
#define YSIM_STUB_TASK%0[%3](%1)   YSIM_PARSER(YSIM_TG)%0(%1)
#define YSIM_STUB_PTSK%0[%3](%1)   YSIM_PARSER(YSIM_TG)%0(%1)
#define YSIM_STUB_FUNC%0(%1)<%9>   YSIM_PARSER(YSIM_TG)%0(%1)
#define YSIM_STUB_GRUP%0(%1)       YSIM_PARSER(YSIM_TG)%0(%1)
#define YSIM_STUB_YCMD%0(%1)       YSIM_PARSER(YSIM_TG)%0(%1)
#define YSIM_STUB_ZERO%0(%1)       YSIM_PARSER(YSIM_TG)%0(%1)
#define stub_body_%0\32; stub_body_

// If there is no master system, these are just forwards/declarations.
#define YSIM_NONE_FOREIGN
#define YSIM_NONE_GLOBAL
#define YSIM_NONE_HOOK%0(%1)       HOOK__%0(%1)
#define YSIM_NONE_TASK%0[%3](%1)   task%0[%3](%1)
#define YSIM_NONE_PTSK%0[%3](%1)   ptask%0[%3](%1)
#define YSIM_NONE_FUNC%0(%1)<%9>   forward%0(%1);public%0(%1)
#define YSIM_NONE_GRUP%0(%1)       _GROUP_MAKE_NAME<HOOK__%0...>(%1)
#define YSIM_NONE_YCMD%0(%1)       @yC_%0(%1);@yC_%0(%1)
#define YSIM_NONE_ZERO%0(%1)       @yC_%0(%1);@yC_%0(%1)

// Now the compilation parts.
#define YSIM_NUM(%0,%1,%2)%8$%4#%5(%7)(%9)            %8$%4#%5i(%7,%2)(%9,%1%2)
#define YSIM_STR(%0,%1,%2,%3)%8$%4#%5(%7)(%9)         %8$%4#%5s(%7,%2)(%9,%1%2[%3])
#define YSIM_ARR(%0,%1,%2,%3)%8$%4#%5(%7)(%9)         %8$%4#%5a(%7,%2)(%9,%1%2[%3])
#define YSIM_REF(%0,%1,%2)%8$%4#%5(%7)(%9)(%3)        %8$%4#%5d(%7,%2)(%9,%1%2)(%3,%2)
#define YSIM_NUM_DEF(%0,%1,%2,%9)%8$%4#%5(%7)(%9)     %8$%4#%5i(%7,%2)(%9,%1%2)
#define YSIM_STR_DEF(%0,%1,%2,%3,%9)%8$%4#%5(%7)(%9)  %8$%4#%5s(%7,%2)(%9,%1%2[%3])
#define YSIM_REF_DEF(%0,%1,%2,%9)%8$%4#%5(%7)(%9)(%3) %8$%4#%5d(%7,%2)(%9,%1%2)(%3,%2)
#define YSIM_stock()%8$  %8$stock PP_NULL<>
#define YSIM_static()%8$ %8$static PP_NULL<>

// All the compile types need the same information.  Collect it all in one
// common way, but use it in many different ways.
#define YSIM_MF_NUM     YSIM_NUM 
#define YSIM_MF_STR     YSIM_STR 
#define YSIM_MF_ARR     YSIM_ARR 
#define YSIM_MF_REF     YSIM_REF 
#define YSIM_MF_NUM_DEF YSIM_NUM_DEF
#define YSIM_MF_STR_DEF YSIM_STR_DEF
#define YSIM_MF_REF_DEF YSIM_REF_DEF
#define YSIM_MF_stock   YSIM_stock
#define YSIM_MF_static  YSIM_static

#define YSIM_TG_NUM     YSIM_NUM 
#define YSIM_TG_STR     YSIM_STR 
#define YSIM_TG_ARR     YSIM_ARR 
#define YSIM_TG_REF     YSIM_REF 
#define YSIM_TG_NUM_DEF YSIM_NUM_DEF
#define YSIM_TG_STR_DEF YSIM_STR_DEF
#define YSIM_TG_REF_DEF YSIM_REF_DEF
#define YSIM_TG_stock   YSIM_stock
#define YSIM_TG_static  YSIM_static

#define YSIM_UF_NUM     YSIM_NUM 
#define YSIM_UF_STR     YSIM_STR 
#define YSIM_UF_ARR     YSIM_ARR 
#define YSIM_UF_REF     YSIM_REF 
#define YSIM_UF_NUM_DEF YSIM_NUM_DEF
#define YSIM_UF_STR_DEF YSIM_STR_DEF
#define YSIM_UF_REF_DEF YSIM_REF_DEF
#define YSIM_UF_stock   YSIM_stock
#define YSIM_UF_static  YSIM_static
#define YSIM_UG_NUM     YSIM_NUM 
#define YSIM_UG_STR     YSIM_STR 
#define YSIM_UG_ARR     YSIM_ARR 
#define YSIM_UG_REF     YSIM_REF 
#define YSIM_UG_NUM_DEF YSIM_NUM_DEF
#define YSIM_UG_STR_DEF YSIM_STR_DEF
#define YSIM_UG_REF_DEF YSIM_REF_DEF
#define YSIM_UG_stock   YSIM_stock
#define YSIM_UG_static  YSIM_static

#define YSIM_DF_NUM     YSIM_NUM 
#define YSIM_DF_STR     YSIM_STR 
#define YSIM_DF_ARR     YSIM_ARR 
#define YSIM_DF_REF     YSIM_REF 
#define YSIM_DF_NUM_DEF YSIM_NUM_DEF
#define YSIM_DF_STR_DEF YSIM_STR_DEF
#define YSIM_DF_REF_DEF YSIM_REF_DEF
#define YSIM_DF_stock   YSIM_stock
#define YSIM_DF_static  YSIM_static
#define YSIM_DG_NUM     YSIM_NUM 
#define YSIM_DG_STR     YSIM_STR 
#define YSIM_DG_ARR     YSIM_ARR 
#define YSIM_DG_REF     YSIM_REF 
#define YSIM_DG_NUM_DEF YSIM_NUM_DEF
#define YSIM_DG_STR_DEF YSIM_STR_DEF
#define YSIM_DG_REF_DEF YSIM_REF_DEF
#define YSIM_DG_stock   YSIM_stock
#define YSIM_DG_static  YSIM_static

// This code uses "i" for normal variables and "d" for references.  These are
// both seen by "CallRemoteFunction" as the same thing, but we can
// differentiate them.
//   
//   %0 - Reference parameter names.
//   %1 - The original parameters.
//   %2 - The parameters without "const" or defaults.
//   %3 - Format specifier.
//   %4 - Function qualifiers (stock etc).
//   %5 - Tag.
//   %6 - The parameters without defaults.
//   %7 - The parameters as arguments (just the names).
//   %8 - code-parse required (internal details, "$" delimited).
//   %9 - Function name.
//   
#define YSIM_MF_END(%9)%8$%4(%1)%3(,%7)(,%2)(%0)        %8$%4%9(%1);@@%9(%2)return L@(%9(%7)%0);forward @@%9(%2)
#define YSIM_MF_END_STR(%9)%8$%4(%1)%3(,%7)(,%2)(%0)    %8$%4%9(%1);@@%9(%2)return T@(%9(%7)%0);forward @@%9(%2)
#define YSIM_MF_END_VOD(%9)%8$%4(%1)%3(,%7)(,%2)(%0)    %8$%4%9(%1);@@%9(%2){%9(%7);return L@(0%0);}forward @@%9(%2)
#define YSIM_MF_END_TAG(%5,%9)%8$%4(%1)%3(,%7)(,%2)(%0) %8$%4%5%9(%1);@@%9(%2)return L@(%9(%7)%0);forward @@%9(%2)
#define YSIM_MF_NUL(%9)%8$%4(%1)%3(%7)(%2)(%0)          %8$%4%9();@@%9()return L@(%9());forward @@%9()
#define YSIM_MF_NUL_STR(%9)%8$%4(%1)%3(%7)(%2)(%0)      %8$%4%9();@@%9()return T@(%9());forward @@%9()
#define YSIM_MF_NUL_VOD(%9)%8$%4(%1)%3(%7)(%2)(%0)      %8$%4%9();@@%9(){%9();return L@(0);}forward @@%9()
#define YSIM_MF_NUL_TAG(%5,%9)%8$%4(%1)%3(%7)(%2)(%0)   %8$%4%5%9();@@%9()return L@(%9());forward @@%9()

#define YSIM_UF_END(%9)%8$%4(%1)%3(%7)(%2)(%0)          %8$%4%9(%1)return F@(),D@(#@@%9,%3%7);forward _@%9(%1)
#define YSIM_UF_END_STR(%9)%8$%4(%1)%3(%7)(%2)(%0)      %8$%4%9(%1)return F@(),K@(#@@%9,%3%7);forward _@%9(%1)
#define YSIM_UF_END_VOD(%9)%8$%4(%1)%3(%7)(%2)(%0)      %8$%4%9(%1)F@(),D@(#@@%9,%3%7);forward _@%9(%1)
#define YSIM_UF_END_TAG(%5,%9)%8$%4(%1)%3(%7)(%2)(%0)   %8$%4%5%9(%1)return F@(),%5D@(#@@%9,%3%7);forward %5_@%9(%1)
#define YSIM_UF_NUL     YSIM_UF_END
#define YSIM_UF_NUL_STR YSIM_UF_END_STR
#define YSIM_UF_NUL_VOD YSIM_UF_END_VOD
#define YSIM_UF_NUL_TAG YSIM_UF_END_TAG

#define YSIM_UG_END(%9)%8$%4(%1)%3(%7)(%2)(%0)          %8$%4_@%9(%1);static stock _@%9(%1)
#define YSIM_UG_END_STR YSIM_UG_END
#define YSIM_UG_END_VOD YSIM_UG_END
#define YSIM_UG_END_TAG(%5,%9)%8$%4(%1)%3(%7)(%2)(%0)   %8$%4%5_@%9(%1);static stock %5_@%9(%1)
#define YSIM_UG_NUL     YSIM_UG_END
#define YSIM_UG_NUL_STR YSIM_UG_END
#define YSIM_UG_NUL_VOD YSIM_UG_END
#define YSIM_UG_NUL_TAG YSIM_UG_END_TAG

#define YSIM_DF_END(%9)%8$%4(%1)%3(,%7)(,%2)(%0)        %8$%4%9(%1);@@%9(%2);@@%9(%2)<>return 1
#define YSIM_DF_END_STR(%9)%8$%4(%1)%3(,%7)(,%2)(%0)    %8$%4%9(%1);@@%9(%2);@@%9(%2)<>return 1
#define YSIM_DF_END_VOD(%9)%8$%4(%1)%3(,%7)(,%2)(%0)    %8$%4%9(%1);@@%9(%2);@@%9(%2)<>return 1
#define YSIM_DF_END_TAG(%5,%9)%8$%4(%1)%3(,%7)(,%2)(%0) %8$%4%5%9(%1);@@%9(%2);@@%9(%2)<>return 1
#define YSIM_DF_NUL(%9)%8$%4(%1)%3(%7)(%2)(%0)          %8$%4%9();@@%9();@@%9()<>return 1
#define YSIM_DF_NUL_STR(%9)%8$%4(%1)%3(%7)(%2)(%0)      %8$%4%9();@@%9();@@%9()<>return 1
#define YSIM_DF_NUL_VOD(%9)%8$%4(%1)%3(%7)(%2)(%0)      %8$%4%9();@@%9();@@%9()<>return 1
#define YSIM_DF_NUL_TAG(%5,%9)%8$%4(%1)%3(%7)(%2)(%0)   %8$%4%5%9();@@%9();@@%9()<>return 1

#define YSIM_DG_END(%9)%8$%4(%1)%3(,%7)(,%2)(%0)        %8$%4%9(%1)<>return F@(),D@(#@@%9,%3,%7);@@%9(%2)<_YCM:y>return L@(%9(%7)%0);%9(%1)<_YCM:y>
#define YSIM_DG_END_STR(%9)%8$%4(%1)%3(,%7)(,%2)(%0)    %8$%4%9(%1)<>return F@(),K@(#@@%9,%3,%7);@@%9(%2)<_YCM:y>return T@(%9(%7)%0);%9(%1)<_YCM:y>
#define YSIM_DG_END_VOD(%9)%8$%4(%1)%3(,%7)(,%2)(%0)    %8$%4%9(%1)<>F@(),D@(#@@%9,%3,%7);@@%9(%2)<_YCM:y>{%9(%7);return L@(0%0);}%9(%1)<_YCM:y>
#define YSIM_DG_END_TAG(%5,%9)%8$%4(%1)%3(,%7)(,%2)(%0) %8$%4%5%9(%1)<>return F@(),%5D@(#@@%9,%3,%7);@@%9(%2)<_YCM:y>return L@(%9(%7)%0);%5%9(%1)<_YCM:y>
#define YSIM_DG_NUL(%9)%8$%4(%1)%3(%7)(%2)(%0)          %8$%4%9()<>return F@(),D@(#@@%9,%3);@@%9()<_YCM:y>return L@(%9()%0);%9()<_YCM:y>
#define YSIM_DG_NUL_STR(%9)%8$%4(%1)%3(%7)(%2)(%0)      %8$%4%9()<>return F@(),K@(#@@%9,%3);@@%9()<_YCM:y>return T@(%9()%0);%9()<_YCM:y>
#define YSIM_DG_NUL_VOD(%9)%8$%4(%1)%3(%7)(%2)(%0)      %8$%4%9()<>F@(),D@(#@@%9,%3);@@%9()<_YCM:y>{%9();return L@(0%0);}%9()<_YCM:y>
#define YSIM_DG_NUL_TAG(%5,%9)%8$%4(%1)%3(%7)(%2)(%0)   %8$%4%5%9()<>return F@(),%5D@(#@@%9,%3);@@%9()<_YCM:y>return L@(%9()%0);%5%9()<_YCM:y>

// These pieces of code give undefined errors if you try and use `global` with
// `YSIM_IS_STUB`.
#define YSIM_TG_END(%9)%8$%4(%1)%3(,%7)(,%2)(%0)        %8$%4_@%9(%1);@@%9();@@%9()stub_body_%9();static stock _@%9(%1)
#define YSIM_TG_END_STR YSIM_TG_NUL
#define YSIM_TG_END_VOD YSIM_TG_NUL
#define YSIM_TG_END_TAG(%5,%9)%8$%4(%1)%3(,%7)(,%2)(%0) %8$%4%5_@%9(%1);@@%9();@@%9()stub_body_%9();static stock %5_@%9(%1)
#define YSIM_TG_NUL(%9)%8$%4(%1)%3(%7)(%2)(%0)          %8$%4_@%9();@@%9();@@%9()stub_body_%9();static stock _@%9()
#define YSIM_TG_NUL_STR YSIM_TG_NUL
#define YSIM_TG_NUL_VOD YSIM_TG_NUL
#define YSIM_TG_NUL_TAG(%5,%9)%8$%4(%1)%3(%7)(%2)(%0)   %8$%4%5_@%9();@@%9();@@%9()stub_body_%9();static stock %5_@%9()

#define YSIM_NOT_CLIENT (!YSIM_HAS_MASTER || !_YSIM_IS_CLIENT)

// These were used to optimise the case where the caller ID wasn't required, but
// that is all done in assembly now and there's no need for the extra
// complication.
//#define @global global
//#define @foreign foreign

// Set up the master ID stack.
#define _YSIM_PUSH_INDEX 0 // First position on the stack to push to.

#if !defined MAX_MASTERS
	#define MAX_MASTERS 64 // Default, old value.
#endif

#define HANDOFF(%9) MASTER_FUNC__ MAKE_YCM<HANDOFF_SOURCE...>()<p>

forward Master_Reassert();

enum _E_YCM
{
	_E_YCM@y, // Is the current master.
	_E_YCM@n, // Is not the current master.
	_E_YCM@m, // Dummy state used purely for determining automata in global/foreign macros.
	_E_YCM@p, // Was the master, now shut down and out the master system.
	_E_YCM@u, // Have stolen the mastership, but not yet been handed off too.
	_E_YCM@w  // Was the master, but not yet started looking for a new one.
}

#define _YCM@y (_YCM@==_E_YCM@y)
#define _YCM@n (_YCM@==_E_YCM@n)
#define _YCM@m (_YCM@==_E_YCM@m)
#define _YCM@p (_YCM@==_E_YCM@p)
#define _YCM@u (_YCM@==_E_YCM@u)
#define _YCM@w (_YCM@==_E_YCM@w)

#if !defined YSI_MODULE_PATH
	#define YSI_MODULE_PATH "../modules/"
#endif

#if !defined YSI_NO_MASTER
	stock
		bool:YSI_gModuleShutdown = false;

	static stock
		YSI_g_sModuleName[32];

	static stock const
		YSI_g_scModulePath[] = YSI_MODULE_PATH;

	static
		YSI_g_sMasterCount = 0,
		YSI_g_sMasterData[MAX_MASTERS];

	stock Master_GetModuleName(ret[32])
	{
		ret = YSI_g_sModuleName;
	}
#endif

/*-------------------------------------------------------------------------*//**
 * <returns>
 *  OnScriptInit
 * </returns>
 * <remarks>
 *  Constructor.  Gets the script a master ID.  Now ALWAYS gets an ID, even if
 *  the master system is disabled - doing otherwise is just too complicated.
 * </remarks>
 * <transition keep="true" target="_ALS : _ALS_go" />
 *//*------------------------------------------------------------------------**/

public OnScriptInit()
{
	P:1("Master_OnScriptInit");
	state _ALS : _ALS_go;
	#if !defined YSI_NO_MASTER
		getproperty(8, "", YSIM_STRING + 1, YSI_g_sModuleName);
		deleteproperty(8, "", YSIM_STRING + 1);
		// This will fix `OnRconCommand` for the gamemode, so we can use the `module` commands.  It
		// also takes up a filterscript slot, a fact we can abuse to get swap space while reloading
		// modules (thus, even if you have the maximum modules loaded, you can still reload one).
		y_rconfix_Load();
	#endif
	// Properties get lost between script changes so we need to force a rebuild.
	@_ = 0;
	CallRemoteFunction("Master_Reassert", "");
	new
		masters = getproperty(8, YSIM_MASTER);
	@_ = Cell_GetLowestEmpty(masters);
	setproperty(8, YSIM_MASTER, masters | Master_ID());
	C:1(if (!Master_ID()) P:E("MasterID not assigned"););
	#if !defined YSI_NO_MASTER
		if (Master_ID() && YSI_g_sModuleName[0])
		{
			setproperty(10, YSI_g_sModuleName, Master_ID());
		}
	#endif
	// Make sure this is called before all other YSI initialisations, at least all the ones which
	// use the master system.
	CallLocalFunction("OnMasterSystemInit", "i", Master_ID());
	Master_OnScriptInit();
	return 1;
}

CHAIN_FORWARD:Master_OnScriptInit() = 0;
#if defined _ALS_OnScriptInit
	#undef OnScriptInit
#else
	#define _ALS_OnScriptInit
#endif
#define OnScriptInit(%0) CHAIN_PUBLIC:Master_OnScriptInit(%0)

forward OnMasterSystemInit(id);

forward Master_OnCloseModules();

/*-------------------------------------------------------------------------*//**
 * <returns>
 *  OnScriptExit
 * </returns>
 * <remarks>
 *  Destructor.
 * </remarks>
 *//*------------------------------------------------------------------------**/

public OnScriptExit()
{
	P:1("MasterOnce_OnScriptExit called");
	#if !defined YSI_NO_MASTER
		if (YSI_g_sModuleName[0])
		{
			deleteproperty(10, YSI_g_sModuleName);
		}
		// This is the main gamemode.  All "modules" are firmly attached to this, and should be
		// unloaded with it.  First mark them as unavailable for handoff, then shut them down.
		if (!YSI_FILTERSCRIPT)
		{
			// First call - disable all modules.
			CallRemoteFunction("Master_OnCloseModules", "");
		}
		// Loop through everything this script is master for and call the remote function for it.
		// EXCEPT for this script itself!
		new
			func[4];
		for (new i = 0, j = YSI_g_sMasterCount; i != j; ++i)
		{
			// This is slightly slower for ending and starting scripts, but uses far less heap
			// space, and these values are rarely used, so may as well pack them (which is what has
			// happened here).
			func[0] = YSI_g_sMasterData[i] & 0xFF,
			func[1] = (YSI_g_sMasterData[i] >> 8) & 0xFF,
			func[2] = YSI_g_sMasterData[i] >> 16,
			CallLocalFunction(func, "");
			P:5("Master_OnScriptExit: %d %d %s", i, YSI_g_sMasterCount, func);
			// The properties currently clear instantly, but that may not always be the case.
		}
	#endif
	setproperty(8, YSIM_MASTER, getproperty(8, YSIM_MASTER) & ~Master_ID()),
	Master_OnScriptExit(),
	CallRemoteFunction("OnMasterSystemClose", "i", Master_ID());
	#if !defined YSI_NO_MASTER
		YSI_gModuleShutdown = true;
		if (!YSI_FILTERSCRIPT)
		{
			// Second call - pass off module data.
			setproperty(8, "", YSIM_STRING + 2, "");
			CallRemoteFunction("Master_OnCloseModules", "");

			// Finally shut down all the modules.
			getproperty(8, "", YSIM_STRING + 2, YSI_UNSAFE_HUGE_STRING);
			new
				prev = 0,
				next = 0;
			while ((next = strfind(YSI_UNSAFE_HUGE_STRING, ";", false, prev)) != -1)
			{
				YSI_UNSAFE_HUGE_STRING[next] = '\0';
				format(Q@, sizeof (Q@), "unloadfs %s%s", YSI_g_scModulePath, YSI_UNSAFE_HUGE_STRING[prev]);
				SendRconCommand(Q@);
				format(Q@, sizeof (Q@), "unloadfs ./%s%s", YSI_g_scModulePath, YSI_UNSAFE_HUGE_STRING[prev]);
				SendRconCommand(Q@);
				prev = next + 1;
			}
		}
	#endif
	return 1;
}

CHAIN_FORWARD:Master_OnScriptExit() = 0;
#if defined _ALS_OnScriptExit
	#undef OnScriptExit
#else
	#define _ALS_OnScriptExit
#endif
#define OnScriptExit(%0) CHAIN_PUBLIC:Master_OnScriptExit(%0)
forward OnMasterSystemClose(id);

/*-------------------------------------------------------------------------*//**
 * <returns>
 *  Master_OnCloseModules
 * </returns>
 * <remarks>
 *  Destructor.
 * </remarks>
 *//*------------------------------------------------------------------------**/

#if !defined YSI_NO_MASTER
	public Master_OnCloseModules()
	{
		P:1("Master_OnCloseModules called");
		if (YSI_g_sModuleName[0])
		{
			// This is a module, and thus purely loaded for the sake of gamemodes.  Shut it down.
			new
				func[4],
				prev = YSI_gModuleShutdown;
			YSI_gModuleShutdown = true;
			for (new i = 0, j = YSI_g_sMasterCount; i != j; ++i)
			{
				// This is slightly slower for ending and starting scripts, but uses far less heap
				// space, and these values are rarely used, so may as well pack them (which is what
				// has happened here).
				func[0] = YSI_g_sMasterData[i] & 0xFF,
				func[1] = (YSI_g_sMasterData[i] >> 8) & 0xFF,
				func[2] = YSI_g_sMasterData[i] >> 16,
				CallLocalFunction(func, "");
				P:5("Master_OnScriptExit: %d %d %s", i, YSI_g_sMasterCount, func);
				// The properties currently clear instantly, but that may not always be the case.
			}
			if (prev)
			{
				// Second call.
				getproperty(8, "", YSIM_STRING + 2, YSI_UNSAFE_HUGE_STRING);
				prev = strlen(YSI_UNSAFE_HUGE_STRING);
				strcat(YSI_UNSAFE_HUGE_STRING[prev], YSI_g_sModuleName);
				strcat(YSI_UNSAFE_HUGE_STRING[prev], ";");
				setproperty(8, "", YSIM_STRING + 2, YSI_UNSAFE_HUGE_STRING);
				YSI_g_sMasterCount = 0;
			}
		}
	}

	bool:Master_LoadModule(const module[])
	{
		if (existproperty(10, module))
		{
			P:E("Module \"%s\" already loaded.", module);
		}
		else
		{
			setproperty(8, "", YSIM_STRING + 1, module);
			new
				masters = getproperty(8, YSIM_MASTER);
			format(Q@, sizeof (Q@), "loadfs %s%s", YSI_g_scModulePath, module);
			SendRconCommand(Q@);
			if (masters == getproperty(8, YSIM_MASTER))
			{
				P:F("Could not load module \"%s\"", module);
				return false;
			}
			else
			{
				P:0("Module \"%s\" loaded", module);
			}
			deleteproperty(8, "", YSIM_STRING + 1);
		}
		return true;
	}

	bool:Master_IsModuleLoaded(const module[])
	{
		return !!existproperty(10, module);
	}

	bool:Master_ReloadModule(const module[])
	{
		if (!Master_IsModuleLoaded(module))
		{
			P:W("Module \"%s\" not loaded.", module);
		}
		// Free up a temporary filterscript slot for the swapping.  Handilly, we have one safe.
		y_rconfix_Unload();
		// We can't just use the default `reloads` command, because that will clear memory before
		// the reload, thus we'll loose data.  Instead, we use a hack to load the FS twice at once!
		// We just alternate which is loaded and unloaded statelessly - detect which method
		// succeeded.
		setproperty(8, "", YSIM_STRING + 1, module);
		new
			masters = getproperty(8, YSIM_MASTER);
		format(Q@, sizeof (Q@), "loadfs ./%s%s", YSI_g_scModulePath, module);
		SendRconCommand(Q@);
		if (masters == getproperty(8, YSIM_MASTER))
		{
			// Didn't load, try the alternate version.
			format(Q@, sizeof (Q@), "loadfs %s%s", YSI_g_scModulePath, module);
			SendRconCommand(Q@);
			if (masters == getproperty(8, YSIM_MASTER))
			{
				P:F("Could not reload module \"%s\"", module);
				y_rconfix_Load();
				return false;
			}
			else
			{
				format(Q@, sizeof (Q@), "unloadfs ./%s%s", YSI_g_scModulePath, module);
				SendRconCommand(Q@);
				CallRemoteFunction("Master_Reassert", "");
				P:0("Module \"%s\" (re)loaded.", module);
			}
		}
		else
		{
			// Loaded a second version of the script.  Unload the first version.
			format(Q@, sizeof (Q@), "unloadfs %s%s", YSI_g_scModulePath, module);
			SendRconCommand(Q@);
			CallRemoteFunction("Master_Reassert", "");
			P:0("Module \"%s\" (re)loaded.", module);
		}
		return true;
	}

	bool:Master_UnloadModule(const module[])
	{
		if (existproperty(10, module))
		{
			new
				masters = getproperty(8, YSIM_MASTER);
			format(Q@, sizeof (Q@), "unloadfs %s%s", YSI_g_scModulePath, module);
			SendRconCommand(Q@);
			format(Q@, sizeof (Q@), "unloadfs ./%s%s", YSI_g_scModulePath, module);
			SendRconCommand(Q@);
			CallRemoteFunction("Master_Reassert", "");
			if (masters == getproperty(8, YSIM_MASTER))
			{
				P:F("Could not unload module \"%s\"", module);
				return false;
			}
			else
			{
				P:0("Module \"%s\" unloaded.", module);
				return true;
			}
		}
		else
		{
			P:E("Module \"%s\" not loaded.", module);
			return false;
		}
	}

	/*-------------------------------------------------------------------------*//**
	 * <returns>
	 *  OnRconCommand
	 * </returns>
	 * <remarks>
	 *  Destructor.
	 * </remarks>
	 *//*------------------------------------------------------------------------**/

	forward OnRconCommand(cmd[]);

	public OnRconCommand(cmd[])
	{
		P:1("MasterOnce_OnRconCommand called");
		if (!YSI_FILTERSCRIPT)
		{
			if (!strcmp(cmd, "loadmodule ", true, 11))
			{
				if (cmd[11])
				{
					Master_LoadModule(cmd[11]);
				}
				else
				{
					P:F("No module specified for \"loadmodule\"");
				}
				return 1;
			}
			else if (!strcmp(cmd, "unloadmodule ", true, 13))
			{
				if (cmd[13])
				{
					Master_UnloadModule(cmd[13]);
				}
				else
				{
					P:F("No module specified for \"unloadmodule\"");
				}
				return 1;
			}
			else if (!strcmp(cmd, "reloadmodule ", true, 13))
			{
				if (cmd[13])
				{
					Master_ReloadModule(cmd[13]);
				}
				else
				{
					P:F("No module specified for \"reloadmodule\"");
				}
				return 1;
			}
		}
		return Master_OnRconCommand(cmd);
	}

	CHAIN_FORWARD:Master_OnRconCommand(cmd[]) = 0;
	#if defined _ALS_OnRconCommand
		#undef OnRconCommand
	#else
		#define _ALS_OnRconCommand
	#endif
	#define OnRconCommand(%0) CHAIN_PUBLIC:Master_OnRconCommand(%0)
#endif

/*-------------------------------------------------------------------------*//**
 * <remarks>
 *  Rebuilds the collection of master data whenever a script is restarted.
 * </remarks>
 *//*------------------------------------------------------------------------**/

public Master_Reassert()
{
	// Make sure that the caller parameter is always 0 by default.
	U@(8, YSIM_CALLER, 0);
	if (Master_ID())
	{
		// Read this script's master value.
		setproperty(8, YSIM_MASTER, getproperty(8, YSIM_MASTER) | Master_ID());
		#if !defined YSI_NO_MASTER
			if (YSI_g_sModuleName[0])
			{
				setproperty(10, YSI_g_sModuleName, Master_ID());
			}
			// Readd this script's owned scripts.
			new
				func[4];
			for (new i = 0; i != YSI_g_sMasterCount; ++i)
			{
				// This is slightly slower for ending and starting scripts, but uses far
				// less heap space, and these values are rarely used, so may as well
				// pack them (which is what has happened here).
				func[0] = YSI_g_sMasterData[i] & 0xFF;
				func[1] = (YSI_g_sMasterData[i] >> 8) & 0xFF;
				func[2] = YSI_g_sMasterData[i] >> 16;
				setproperty(9, func, Master_ID());
			}
		#endif
	}
}

#if defined YSI_NO_MASTER
	#endinput
#endif

stock _Master_Relinquish(const library[])
{
	P:4("_Master_Relinquish called: %s %d", library, YSI_g_sMasterCount);
	switch (YSI_g_sMasterCount)
	{
		case 0: return;
		case 1: YSI_g_sMasterCount = 0;
		default:
		{
			new
				m = library[0] | (library[1] << 8) | (library[2] << 16);
			// Last one, don't switch.
			if (YSI_g_sMasterData[--YSI_g_sMasterCount] == m) return;
			// Find where this master is in the list and remove it.
			for (new i = 0; i != YSI_g_sMasterCount; ++i)
			{
				if (YSI_g_sMasterData[i] == m)
				{
					// Shuffle them to same space.
					YSI_g_sMasterData[i] = YSI_g_sMasterData[YSI_g_sMasterCount];
					return;
				}
			}
		}
	}
}

enum e_YSIM_GET_TYPE
{
	e_YSIM_GET_TYPE_NO_CONTEST,
	e_YSIM_GET_TYPE_STOLE,
	e_YSIM_GET_TYPE_NOT_GOT,
}

/*-------------------------------------------------------------------------*//**
 * <param name="library">The name of the library to try become master for.</param>
 *//*------------------------------------------------------------------------**/

static stock const
	YSI_g_scYSIM_VERSION[] = "YSIM_VERSION%s";

stock e_YSIM_GET_TYPE:_Master_Get(const library[], bool:force = false)
{
	P:2("_Master_Get called");
	new
		e_YSIM_GET_TYPE:ret = e_YSIM_GET_TYPE_NO_CONTEST;
	new
		master = getproperty(9, library);
	P:3("E_YSIM_GET_TYPE:_Master_Get called: \"%s\", %i, %d", library, _:force, master);
	if (master)
	{
		P:4("_Master_Get: Prop exists: %d %d", master, Master_ID());
		P:4("_Master_Get: Prop set");
		if (master == Master_ID())
		{
			// Nothing has changed.
			return e_YSIM_GET_TYPE_NO_CONTEST;
		}
		else if (!force)
		{
			new
				func[32];
			format(func, sizeof (func), YSI_g_scYSIM_VERSION, library);
			master = CallLocalFunction(func, "");
			// Might still be able to steal the master if we're a higher version.  We must be
			// EXCLUSIVELY higher, not just same or higher.
			CallRemoteFunction(func, "");
			if (master <= getproperty(8, YSIM_VERRET))
			{
				// Not got the master.
				return e_YSIM_GET_TYPE_NOT_GOT;
			}
		}
		// A server is forcing itself.
		ret = e_YSIM_GET_TYPE_STOLE;
	}
	else if (!force && existproperty(8, YSIM_VERMAX))
	{
		new
			func[32];
		format(func, sizeof (func), YSI_g_scYSIM_VERSION, library);
		if (CallLocalFunction(func, "") < getproperty(8, YSIM_VERMAX))
		{
			// There's another pending library with a higher version than us.  If we're equal, its
			// first-come, first-served.
			return e_YSIM_GET_TYPE_NOT_GOT;
		}
	}
	P:4("_Master_Get: Get master.");
	setproperty(9, library, Master_ID());
	P:4("_Master_Get: Set master.");
	// Add this library to the list.  The list is designed to only deal with
	// two or three character master names now!
	if (YSI_g_sMasterCount < MAX_MASTERS)
	{
		P:4("_Master_Get: Set master string %s = %d", library, library[0] | (library[1] << 8) | (library[2] << 16));
		YSI_g_sMasterData[YSI_g_sMasterCount++] = library[0] | (library[1] << 8) | (library[2] << 16);
		P:4("_Master_Get: Set master string.");
	}
	P:C(else P:E("Too many master scripts"););
	return ret;
}

#if defined @aOnScriptInit
	forward @aOnScriptInit(id);
#endif
#if defined @bOnScriptInit
	forward @bOnScriptInit(id);
#endif
#if defined @cOnScriptInit
	forward @cOnScriptInit(id);
#endif
#if defined @dOnScriptInit
	forward @dOnScriptInit(id);
#endif
#if defined @eOnScriptInit
	forward @eOnScriptInit(id);
#endif
#if defined @fOnScriptInit
	forward @fOnScriptInit(id);
#endif
#if defined @gOnScriptInit
	forward @gOnScriptInit(id);
#endif
#if defined @hOnScriptInit
	forward @hOnScriptInit(id);
#endif
#if defined @iOnScriptInit
	forward @iOnScriptInit(id);
#endif
#if defined @jOnScriptInit
	forward @jOnScriptInit(id);
#endif
#if defined @kOnScriptInit
	forward @kOnScriptInit(id);
#endif
#if defined @lOnScriptInit
	forward @lOnScriptInit(id);
#endif
#if defined @mOnScriptInit
	forward @mOnScriptInit(id);
#endif
#if defined @nOnScriptInit
	forward @nOnScriptInit(id);
#endif
#if defined @oOnScriptInit
	forward @oOnScriptInit(id);
#endif
#if defined @pOnScriptInit
	forward @pOnScriptInit(id);
#endif
#if defined @qOnScriptInit
	forward @qOnScriptInit(id);
#endif
#if defined @rOnScriptInit
	forward @rOnScriptInit(id);
#endif
#if defined @sOnScriptInit
	forward @sOnScriptInit(id);
#endif
#if defined @tOnScriptInit
	forward @tOnScriptInit(id);
#endif
#if defined @uOnScriptInit
	forward @uOnScriptInit(id);
#endif
#if defined @vOnScriptInit
	forward @vOnScriptInit(id);
#endif
#if defined @wOnScriptInit
	forward @wOnScriptInit(id);
#endif
#if defined @xOnScriptInit
	forward @xOnScriptInit(id);
#endif
#if defined @yOnScriptInit
	forward @yOnScriptInit(id);
#endif
#if defined @zOnScriptInit
	forward @zOnScriptInit(id);
#endif
#if defined @AOnScriptInit
	forward @AOnScriptInit(id);
#endif
#if defined @BOnScriptInit
	forward @BOnScriptInit(id);
#endif
#if defined @COnScriptInit
	forward @COnScriptInit(id);
#endif
#if defined @DOnScriptInit
	forward @DOnScriptInit(id);
#endif
#if defined @EOnScriptInit
	forward @EOnScriptInit(id);
#endif
#if defined @FOnScriptInit
	forward @FOnScriptInit(id);
#endif
#if defined @GOnScriptInit
	forward @GOnScriptInit(id);
#endif
#if defined @HOnScriptInit
	forward @HOnScriptInit(id);
#endif
#if defined @IOnScriptInit
	forward @IOnScriptInit(id);
#endif
#if defined @JOnScriptInit
	forward @JOnScriptInit(id);
#endif
#if defined @KOnScriptInit
	forward @KOnScriptInit(id);
#endif
#if defined @LOnScriptInit
	forward @LOnScriptInit(id);
#endif
#if defined @MOnScriptInit
	forward @MOnScriptInit(id);
#endif
#if defined @NOnScriptInit
	forward @NOnScriptInit(id);
#endif
#if defined @OOnScriptInit
	forward @OOnScriptInit(id);
#endif
#if defined @POnScriptInit
	forward @POnScriptInit(id);
#endif
#if defined @QOnScriptInit
	forward @QOnScriptInit(id);
#endif
#if defined @ROnScriptInit
	forward @ROnScriptInit(id);
#endif
#if defined @SOnScriptInit
	forward @SOnScriptInit(id);
#endif
#if defined @TOnScriptInit
	forward @TOnScriptInit(id);
#endif
#if defined @UOnScriptInit
	forward @UOnScriptInit(id);
#endif
#if defined @VOnScriptInit
	forward @VOnScriptInit(id);
#endif
#if defined @WOnScriptInit
	forward @WOnScriptInit(id);
#endif
#if defined @XOnScriptInit
	forward @XOnScriptInit(id);
#endif
#if defined @YOnScriptInit
	forward @YOnScriptInit(id);
#endif
#if defined @ZOnScriptInit
	forward @ZOnScriptInit(id);
#endif
#if defined @0OnScriptInit
	forward @0OnScriptInit(id);
#endif
#if defined @1OnScriptInit
	forward @1OnScriptInit(id);
#endif
#if defined @2OnScriptInit
	forward @2OnScriptInit(id);
#endif
#if defined @3OnScriptInit
	forward @3OnScriptInit(id);
#endif
#if defined @4OnScriptInit
	forward @4OnScriptInit(id);
#endif
#if defined @5OnScriptInit
	forward @5OnScriptInit(id);
#endif
#if defined @6OnScriptInit
	forward @6OnScriptInit(id);
#endif
#if defined @7OnScriptInit
	forward @7OnScriptInit(id);
#endif
#if defined @8OnScriptInit
	forward @8OnScriptInit(id);
#endif
#if defined @9OnScriptInit
	forward @9OnScriptInit(id);
#endif

