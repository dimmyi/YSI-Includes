/*                                                                                              *
 *                                                                                             *
 *     _____ ______ ______   ____  ______ _      ______          __  ______ ____  _____        *
 *    / ____|  ____|  ____| |  _ \|  ____| |    / __ \ \        / / |  ____/ __ \|  __ \       *
 *   | (___ | |__  | |__    | |_) | |__  | |   | |  | \ \  /\  / /  | |__ | |  | | |__) |      *
 *    \___ \|  __| |  __|   |  _ <|  __| | |   | |  | |\ \/  \/ /   |  __|| |  | |  _  /       *
 *    ____) | |____| |____  | |_) | |____| |___| |__| | \  /\  /    | |   | |__| | | \ \       *
 *   |_____/|______|______| |____/|______|______\____/   \/  \/     |_|    \____/|_|  \_\      *
 *                                                                                             *
 *    _____   ____   _____ _    _ __  __ ______ _   _ _______    _______ _____ ____  _   _     *
 *   |  __ \ / __ \ / ____| |  | |  \/  |  ____| \ | |__   __|/\|__   __|_   _/ __ \| \ | |    *
 *   | |  | | |  | | |    | |  | | \  / | |__  |  \| |  | |  /  \  | |    | || |  | |  \| |    *
 *   | |  | | |  | | |    | |  | | |\/| |  __| | . ` |  | | / /\ \ | |    | || |  | | . ` |    *
 *   | |__| | |__| | |____| |__| | |  | | |____| |\  |  | |/ ____ \| |   _| || |__| | |\  |    *
 *   |_____/ \____/ \_____|\____/|_|  |_|______|_| \_|  |_/_/    \_\_|  |_____\____/|_| \_|    *
 *                                                                                             *
 *   This is required for technical reasons - to place it after `#endinput` to not generate    *
 *                   multiple copies of it in XML when compiling with `-r`.                    *
 *                                                                                             *
*//*
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
#if !defined _INC_y_master
	#include "..\..\YSI_Internal\y_version"
	#include "..\y_debug"
	#include "..\..\YSI_Coding\y_hooks"
	#include "..\..\YSI_Internal\y_shortfunc"
	
	#include "..\..\YSI_Internal\y_natives"
	#include "..\..\YSI_Internal\y_prehook"
	
	CHAIN_HOOK(Master)
	#undef CHAIN_ORDER
	#define CHAIN_ORDER CHAIN_NEXT(Master)
	
	#include "y_master_once"
#endif

// ALWAYS!  The compiler defines this automatically, don't use it as this is
// included multiple times to handle multiple master systems.
#if defined _inc_y_master
	#undef _inc_y_master
#endif

#if defined MASTER
	#if defined _MASTER
		// Store the last master.
		#include "y_master\_push_master"
	#endif
#else
	// Get the previous master (or 0 by default).
	#include "y_master\_pop_master"
	
	// Useful debug information (isn't compiled, but shows up with "-l").
	P:C(static stock const _POPPED_MASTER = MASTER;);
#endif

#if MASTER < 0 || MASTER >= MAX_MASTERS
	#error Invalid MASTER ID.
#endif

#if defined _INC_y_master
	// Run at least once - clear previous data.
	#include "y_master\_cleanup"
#endif

// Initialise the current master if it hasn't already been done.  This is split
// in to 5 parts to reduce compilation times in the common case of not that many
// master systems.
#if     MASTER < 16
	#include "y_master\_setup_master_A"
#elseif MASTER < 32
	#include "y_master\_setup_master_B"
#elseif MASTER < 48
	#include "y_master\_setup_master_C"
#else
	#include "y_master\_setup_master_D"
#endif
#undef MASTER

// Include all the correct macro versions for the settings we have just found.
#include "y_master\_impl"

#if defined _INC_y_master
	#endinput
#endif
#define _INC_y_master

/**
 * <library name="y_master">
 * <section>
 *  Description
 * </section>
 *  Handles distribution of control of code segments across all running scripts
 *  with YSI to avoid conflicts of checkpoint streamers etc and allow features
 *  like "/commands" displaying all commands on the server.
 *
 *  Now fully autonomous - to use the master system simply define which one you
 *  want and include the script:
 *
 *  <c>
 *    #define MASTER 14
 *    #include "Master"
 *  </c>
 *
 *  That will make the current script attempt to make itself the main master -
 *  you don't need ANY other code for initialisation anywhere (which took a
 *  while to figure out)...
 *
 *  This is now over 2000 lines of repetitive and complex macros!
 * <section>
 *  Version
 * </section>
 *  0.2
 * <section>
 *  Macros
 * </section><ul>
 *  <symbol name="RF">Declare a function which may be remote.</symbol>
 *  <symbol name="RV">Declare a void function which may be remote.</symbol>
 *  <symbol name="RS">Declare an optional callback.</symbol>
 *  <symbol name="RFN">Declare an optional callback with no parameters.</symbol>
 *  <symbol name="RFC">Declare a slower but shorter function.</symbol>
 *  <symbol name="RFP">Declare a function with tags and give recall hints.</symbol>
 *  <symbol name="RFPC">Combination of P and C.</symbol>
 *  <symbol name="RFCP">Combination of P and C.</symbol>
 * </ul><section>
 *  Variables
 * </section>
 * <subsection>
 * Global
 * </subsection><ul>
 *  <symbol name="@_">ID of this script.</symbol>
 * </ul><section>Compile options</section><ul>
 *  <symbol name="YSI_NO_MASTER">Don't use the master system.</symbol>
 *  <symbol name="YSI_IS_CLIENT">Script is a client only script.</symbol>
 *  <symbol name="YSI_IS_SERVER">Script is a server only script.</symbol>
 * </ul>
 * </library>
 *//** */

// Only include this lot once.
#if defined YSI_TESTS
	#include "..\..\YSI_Core\y_testing"
	// Get the new master.
	#define MASTER 58
	#include "..\y_master"
	// Include the tests.
	#include "y_master_tests"
	// Pop the master.
	#include "..\y_master"
#endif
