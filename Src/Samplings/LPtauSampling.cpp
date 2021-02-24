// ************************************************************************
// Copyright (c) 2007   Lawrence Livermore National Security, LLC. 
// Produced at the Lawrence Livermore National Laboratory.
// Written by the PSUADE team.
// All rights reserved.
//
// Please see the COPYRIGHT_and_LICENSE file for the copyright notice,
// disclaimer, contact information and the GNU Lesser General Public License.
//
// PSUADE is free software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License (as published by the Free Software
// Foundation) version 2.1 dated February 1999.
//
// PSUADE is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the IMPLIED WARRANTY OF MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the terms and conditions of the GNU General
// Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
// ************************************************************************
// Functions for the LPtau class 
// AUTHOR : CHARLES TONG
// DATE   : 2004
// ************************************************************************
//    The acpo code was authored by
//    B. Shukhman (GENERATION OF QUASI-RANDOM (LPTAU) VECTORS FOR PARALLEL
//    COMPUTATION.  B. SHUKHMAN. . IN COMP. PHYS. COMMUN. 78 (1994) 279
// ************************************************************************

// ------------------------------------------------------------------------
// system and local includes
// ------------------------------------------------------------------------
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sysdef.h"
#include "PsuadeUtil.h"
#include "LPtauSampling.h"

// ------------------------------------------------------------------------
// local definitions
// ------------------------------------------------------------------------
#define  PSUADE_ACPO_MaxDimension  51  
#define  PSUADE_ACPO_MaxNSamples   1073741823   
#define  PSUADE_ACPO_QuantityPower 30 

static unsigned long
PSUADE_ACPO_Directing[PSUADE_ACPO_QuantityPower][PSUADE_ACPO_MaxDimension] =
{         
   {536870912,    536870912,    536870912,    536870912,    536870912,
    536870912,    536870912,    536870912,    536870912,    536870912,
    536870912,    536870912,    536870912,    536870912,    536870912,
    536870912,    536870912,    536870912,    536870912,    536870912,
    536870912,    536870912,    536870912,    536870912,    536870912,
    536870912,    536870912,    536870912,    536870912,    536870912,
    536870912,    536870912,    536870912,    536870912,    536870912,
    536870912,    536870912,    536870912,    536870912,    536870912,
    536870912,    536870912,    536870912,    536870912,    536870912,
    536870912,    536870912,    536870912,    536870912,    536870912,
    536870912 },                                                      
                                                                        
   {805306368,    268435456,    805306368,    268435456,    805306368,
    268435456,    805306368,    268435456,    268435456,    805306368,
    268435456,    805306368,    268435456,    805306368,    268435456,
    805306368,    805306368,    268435456,    805306368,    268435456,
    805306368,    268435456,    805306368,    268435456,    268435456,
    805306368,    268435456,    805306368,    268435456,    805306368,
    268435456,    805306368,    805306368,    268435456,    805306368,
    268435456,    805306368,    268435456,    805306368,    268435456,
    268435456,    805306368,    268435456,    805306368,    268435456,
    805306368,    268435456,    805306368,    805306368,    268435456,
    805306368},                                                       
                                                                        
   {939524096,    939524096,    134217728,    671088640,    402653184,
    402653184,    671088640,    134217728,    671088640,    402653184,
    939524096,    134217728,    671088640,    939524096,    134217728,
    671088640,    134217728,    939524096,    939524096,    402653184,
    402653184,    134217728,    671088640,    402653184,    671088640,
    402653184,    402653184,    671088640,    134217728,    134217728,
    939524096,    939524096,    939524096,    939524096,    134217728,
    671088640,    402653184,    402653184,    671088640,    134217728,
    671088640,    402653184,    939524096,    134217728,    671088640,
    939524096,    134217728,    671088640,    134217728,    939524096,
    939524096},                                                       
                                                                        
   {1006632960,    67108864,    603979776,   1006632960,    335544320,
     469762048,    872415232,   738197504,    469762048,    872415232,
    1006632960,    67108864,    872415232,    469762048,    469762048,
     469762048,   1006632960,   335544320,    872415232,    603979776,
    201326592,     67108864,    335544320,    67108864,     201326592,
     738197504,   1006632960,   738197504,    603979776,    335544320,
     738197504,    67108864,   1006632960,    67108864,     603979776,
    1006632960,    335544320,   469762048,    872415232,    738197504,
     469762048,    872415232,  1006632960,    67108864,     872415232,
     469762048,    469762048,   469762048,   1006632960,    335544320,
     872415232},                                                       
                                                                        
   {1040187392,    637534208,   1040187392,    838860800,    167772160,
     234881024,    167772160,   1040187392,    436207616,     33554432,
     570425344,   1040187392,    503316480,    838860800,    973078528,
     167772160,    234881024,    436207616,    771751936,    100663296,
     234881024,    905969664,    771751936,     33554432,    838860800,
     973078528,    905969664,     33554432,    301989888,    838860800,
     100663296,    234881024,   1040187392,    637534208,   1040187392,
     838860800,    167772160,    234881024,    167772160,   1040187392,
     436207616,     33554432,    570425344,   1040187392,    503316480,
     838860800,    973078528,    167772160,    234881024,    436207616,
     771751936},                                                       
                                                                        
   {1056964608,    352321536,     50331648,    419430400,    956301312,
     889192448,    620756992,    117440512,    956301312,    922746880,
     822083584,    218103808,    587202560,    385875968,    452984832,
     218103808,    184549376,    285212672,    150994944,    956301312,
     352321536,    654311424,    553648128,    352321536,    788529152,
     956301312,    587202560,    251658240,    218103808,    721420288,
     251658240,   1056964608,    520093696,    889192448,    587202560,
     956301312,    419430400,    352321536,     83886080,    654311424,
     419430400,    385875968,    285212672,    754974720,     50331648,
     922746880,    989855744,    754974720,    721420288,    822083584,
     687865856},                                                       
                                                                        
   {1065353216,   1065353216,    578813952,     25165824,    125829120,
     964689920,    327155712,    310378496,    360710144,    360710144,
     159383552,    444596224,    947912704,    662700032,    444596224,
     528482304,    578813952,    578813952,     75497472,   1065353216,
      92274688,    511705088,    897581056,    847249408,    662700032,
     931135488,    411041792,    713031680,    696254464,    528482304,
     209715200,    578813952,   1065353216,   1065353216,    578813952,
      25165824,    125829120,    964689920,    327155712,    310378496,
     360710144,    360710144,    159383552,    444596224,    947912704,
      662700032,    444596224,    528482304,    578813952,    578813952,
      75497472},                                                       
                                                                        
   {1069547520,      4194304,    826277888,    624951296,    616562688,
     801112064,    952107008,    406847488,    398458880,    331350016,
     356515840,     46137344,   1010827264,   1069547520,    734003200,
     113246208,    490733568,    633339904,    910163968,    801112064,
     893386752,    851443712,    801112064,    918552576,    742391808,
     499122176,     62914560,    264241152,    708837376,    717225984,
     759169024,    499122176,    272629760,    423624704,    155189248,
     239075328,    205520896,    943718400,    373293056,    457179136,
     406847488,     71303168,    473956352,     54525952,    624951296,
     104857600,   1019215872,    901775360,    213909504,    281018368,
     851443712},                                                       
                                                                        
   {1071644672,    543162368,    190840832,    329252864,    853540864,
     132120576,    778043392,     73400320,    178257920,    522190848,
     639631360,    534773760,    991952896,    333447168,     48234496,
    1059061760,    761266176,    673185792,    220200960,    396361728,
     362807296,    815792128,    819986432,    346030080,     39845888,
     752877568,    387973120,    643825664,    291504128,    274726912,
     568328192,    526385152,    673185792,     98566144,    396361728,
     727711744,   1042284544,    106954752,    299892736,    912261120,
      44040192,    895483904,    333447168,    551550976,    467664896,
     618659840,    606076928,    274726912,    245366784,    446693376,
     421527552},                                                       
                                                                        
   {1072693248,    273678336,    644874240,    753926144,    495976448,
     869269504,    355467264,     57671680,    816840704,    961544192,
     804257792,    495976448,    347078656,    426770432,   1066401792,
     372244480,     84934656,    208666624,    313524224,    598736896,
     487587840,    965738496,   1011875840,    296747008,    393216000,
     523239424,    720371712,    823132160,    128974848,    407896064,
     747634688,    850395136,    873463808,    504365056,    481296384,
     686817280,    592445440,    995098624,    498073600,    969932800,
     586153984,   1039138816,    814743552,    523239424,    294649856,
     305135616,    506462208,     11534336,    449839104,    619708416,
     479199232},                                                       
                                                                        
   {1073217536,    947388416,   1070071808,    977797120,    365428736,
     702021632,    461897728,    829947904,    425197568,    634912768,
     437780480,    582483968,    792199168,    315097088,    611844096,
     667418624,    166199296,    513277952,    187170816,   1036517376,
      25690112,    201850880,    443023360,    990380032,     63438848,
     211288064,    983040000,   1069023232,    421003264,    742916096,
     487063552,    363331584,    973602816,    286785536,    171442176,
     669515776,    110624768,    383254528,    289931264,    352845824,
     878182400,    655884288,    836239360,    765984768,    549978112,
     655884288,     85458944,    591921152,    563609600,    277348352,
     919076864},                                                       
                                                                        
   {1073479680,     71565312,      2359296,    891551744,    158597120,
     383516672,   1019478016,    947126272,    621019136,    714866688,
     738459648,    265027584,    468975616,    131858432,    504627200,
     581173248,    266600448,    865861632,    658243584,    546045952,
     521404416,    304873472,   1060896768,    163840000,    305922048,
     257163264,     50069504,    773062656,     59506688,    779354112,
     165937152,    587988992,    486801408,    160694272,     90439680,
     423362560,    536608768,    614203392,     56885248,    999030784,
      10747904,    764674048,     25952256,    989069312,    352583680,
     799801344,    261357568,    873201664,     40108032,    769392640,
     254541824},                                                       
                                                                        
   {1073610752,    644218880,    538836992,    455475200,   1062600704,
     139329536,    205651968,    905052160,    797048832,    452329472,
     973471744,    627703808,    614072320,    803078144,    637403136,
     835059712,    949878784,    662044672,    767950848,    426901504,
     448659456,     23986176,   1016201216,    524943360,    525991936,
     618790912,    781058048,    761659392,    458096640,    226361344,
     950665216,    952500224,    516030464,    337510400,    496107520,
     830865408,    944111616,    636354560,    978452480,    921567232,
     533594112,      7471104,    678035456,    471203840,   1065746432,
     575275008,    996540416,    909246464,    879362048,    637927424,
      25821184},                                                       
                                                                        
   {1073676288,    357892096,    808648704,      2424832,      2555904,
     624230400,     69271552,    456851456,   1052966912,    600637440,
     487260160,    794624000,    386727936,    467599360,    798031872,
     630652928,    340983808,    493944832,     37945344,    264175616,
     263520256,    833421312,    235077632,    464846848,    534839296,
     992411648,     10813440,    367067136,    116457472,    115015680,
     928710656,    619773952,    813760512,   1043398656,    967770112,
     912850944,     72155136,   1009057792,    668532736,    462356480,
     267321344,    795803648,    635764736,    574160896,   1003421696,
     181075968,     56688640,    388562944,    190906368,    657915904,
     474939392},                                                       
                                                                        
   {1073709056,   1073709056,    137003008,    547782656,    545095680,
      26836992,     34701312,    354385920,    925663232,    656965632,
     327581696,    894795776,    110067712,   1038057472,    209354752,
     596541440,     42631168,    471433216,     52527104,    666861568,
     706707456,    674070528,    824410112,    305496064,    136282112,
     847740928,    531464192,    222920704,    379289600,    507740160,
      11894784,   1053392896,    129990656,    557547520,    666468352,
    1061912576,    576684032,   1041334272,    380469248,    114196480,
     133070848,    517046272,    129990656,    790396928,    563773440,
     388333568,    661749760,    446791680,    737378304,    229998592,
     348225536},                                                       
                                                                        
   {1073725440,        16384,    605372416,    275234816,    817971200,
     603963392,    555335680,    721534976,    997801984,   1028767744,
     407060480,    375275520,    256688128,   1021165568,    303349760,
    1022476288,    234143744,    106708992,    732971008,    733954048,
     789889024,    879575040,    764657664,    762658816,   1010843648,
     941080576,    827932672,     98942976,   1051738112,    624934912,
     993280000,    134070272,    201375744,    567558144,    882163712,
     649084928,    356564992,    489439232,    637091840,     60637184,
     199278592,    815677440,    927678464,     94519296,    419184640,
     933838848,    426655744,    911130624,    171393024,    561332224,
     471613440},                                                       

   {1073733632,    536895488,   1043685376,    679944192,    417505280,
     301981696,    832561152,    210542592,    167501824,   1071341568,
     229302272,    970661888,    732176384,    576659456,    402464768,
     451584000,    368467968,    928260096,    933847040,     29319168,
     582934528,    772612096,    330014720,    647323648,    174071808,
    1008689152,    295919616,    353869824,    177774592,    580198400,
     381837312,    638574592,    637558784,    679370752,    504012800,
     747118592,    429973504,   1032609792,    932667392,    583360512,
     969498624,   1056333824,    660955136,    247488512,    153509888,
     242180096,    205840384,    797499392,    824565760,    234348544,
     842326016},                                                       

   {1073737728,    268455936,     52785152,   1020628992,    345018368,
     452972544,    704442368,    255987712,    750759936,    697692160,
     196677632,    764604416,    485625856,    522022912,    680620032,
     362270720,    838103040,     83972096,    629133312,     46108672,
     867561472,    725422080,    184504320,    751112192,    191918080,
     306425856,    507310080,     30453760,    281858048,    604000256,
     208662528,    319557632,    318779392,    476139520,    863719424,
     567062528,    521179136,    712790016,    610299904,    293687296,
    1023086592,    549089280,   1065242624,    707751936,    363024384,
      16674816,    197136384,   1037561856,    195112960,    372707328,
     992751616},                                                       

   {1073739776,    939554816,    580732928,    854333440,    172619776,
     511694848,    936142848,    518199296,    593348608,    225527808,
     900982784,    180279296,    168904704,     62814208,    754485248,
     730691584,   1005996032,    411174912,    249866240,    641669120,
    1008719872,    749066240,    860993536,     94177280,    432564224,
     226355200,    925784064,    995657728,    967731200,    436226048,
     913799168,    549894144,    964696064,    843315200,    445863936,
    1047422976,    548947968,    492066816,    953870336,   1002653696,
     861440000,    385636352,    325253120,    187353088,    653584384,
    1008269312,    748693504,   1013016576,     55814144,    255170560,
     260708352},                                                       
                                                                        
   {1073740800,     67126272,    829514752,    423777280,    968297472,
     205511680,    147076096,    926669824,    202300416,    118395904,
     381332480,   1002738688,    743042048,    292551680,    584567808,
     284339200,    183936000,    616762368,    435221504,    159376384,
     907322368,    595696640,    247497728,    553735168,    826051584,
     564454400,    446024704,    214236160,     33661952,    251685888,
     660327424,    284244992,    859868160,    722502656,    622844928,
     324342784,    682374144,    400579584,    405353472,    605187072,
     840682496,    212956160,    157891584,    193201152,    437990400,
     573578240,    368053248,    580197376,    937905152,    565527552,
      89064448},                                                       
                                                                        
   {1073741312,    637560320,    189496832,     27474432,    129338880,
     908054016,    641870336,    186375680,    302677504,    763663872,
     103878144,    325187072,    858254848,    922041856,    261924352,
     954978816,    292822528,    849512960,    210311680,    933232128,
     691981824,    155417088,    627070464,    416795136,    182081024,
     513433088,    848658944,    515770880,    627273216,    629169664,
     414566912,    147450368,    698353152,    244844032,    226578944,
    1020087808,    886978048,    389697024,   1007004160,    839646720,
     621924864,    549962240,    609583616,    735976960,     87342592,
    1058542080,    163066368,    307997184,    876471808,    794280448,
     675386880},                                                       
                                                                        
   {1073741568,    352343296,    644236032,    636735232,    615860480,
     959444224,    287380736,   1007410432,    890187008,    399480576,
     520092928,    643311360,    816901376,    695310080,   1019229440,
      77034240,    733295872,   1035127552,    986582784,    332381952,
     334852352,    364956416,    596672256,    800381696,    480316672,
     574863104,    647347968,    702910208,    499965184,    364968704,
     120862976,   1023256320,    995114240,     13951232,     32520448,
     702127360,     45176064,    444945664,    237860096,    152839936,
     530633984,    429135616,    267272448,    884808960,    933712640,
      61605632,    174335744,    564911360,    302327552,    650589440,
     450649344},                                                       
                                                                        
   {1073741696,   1065385856,   1073734272,    331949184,    842310784,
     799537536,    965852032,    369351808,    662886016,     86119808,
     865109888,    299633792,    422735488,    181087360,    174252416,
    1041212544,    840196224,    750314368,    391053440,    903306880,
     742365312,    236995200,     42492800,    946000512,    771692416,
     897405824,    613803136,    924258688,    808338304,   1038125440,
     683814272,    177186176,    766008960,    704549248,    194555008,
     306383744,    496592512,    416020864,    655186816,   1032204928,
     694773632,    577910144,     45797760,    910332544,    536014976,
     675946368,    987635840,    788223872,    353993856,     96313472,
      85248640},                                                       
                                                                        
   {1073741760,      4210752,        12608,    744788544,    494377792,
     115601344,    769248448,    990895808,    851706304,    979326784,
     692061120,    429015104,    217132864,    736067008,     55694400,
     456152640,    631601984,    787264192,    898599104,    478383808,
     507774272,    458270272,    392995136,    872482496,    124824768,
    1034161344,    362141632,   1053833280,    943810496,    428920128,
     795835200,    835462848,    961843520,    606198080,    785652672,
     154954432,    491617344,    297351232,    580735552,    634587968,
     185277760,    141505600,    417673280,    106907456,    395575616,
     566958656,    352651200,    415242688,     26635200,   1030317504,
     247212992},                                                       
                                                                        
   {1073741792,    543187040,    536882016,    981766752,    357858144,
     810665952,    369083488,    613015520,     86115232,    845149216,
     606076960,   1018241504,     35245536,    635403744,    236633696,
     407451232,    427671904,    460141792,    116344544,    990246688,
    1024892576,    883429408,    150655392,    173476896,    197666464,
    1016740448,    605376608,    970487008,   1006881248,    598265632,
    1022861728,    489055392,    216680608,    371439072,     53140576,
     965114400,     98534112,    209692256,    264598496,    321437280,
     545303840,    324119904,    876587488,    778239712,    802033120,
      44406496,    665829024,    803213920,    309742112,    735181344,
    1036125600},                                                       
                                                                        
   {1073741808,    273698896,    805312112,    903123536,    153504624,
     689632208,    432848944,    859888752,    510853776,    240805744,
     250610192,    852489168,    139460144,    283082224,    222702928,
     342181488,    922872432,    187528656,    360637424,    814514736,
     301037840,    800872624,    272595184,    158627600,    238839088,
     927181136,    710248688,    788854608,   1048376560,    484709072,
      85709008,   1065469744,    429237328,    490778384,    848024144,
     443114288,    687907504,    474573648,     45706416,    681465296,
     805303216,     14567920,    369839504,    440402480,    797652624,
     115959088,    929784560,     91226544,    205943056,    288358704,
     950217296},                                                       
                                                                        
   {1073741816,    947419256,    134232008,    460100200,   1073685336,
     398354904,   1069834248,    686054696,    108299224,    273898840,
     731382552,    938170664,     86812552,    677346808,    602208712,
     652635752,    209797064,    323968376,    384078968,    561178056,
     923399256,    996597176,    942777416,    885167400,     89791048,
     956122504,     87393464,    987661336,    993412952,    827749496,
     903211272,     33649816,    594875176,     65052056,    822835240,
     625704888,   1065374584,    612232920,    536299720,   1046858504,
     939518840,    160843032,    654656088,    744496936,    872197704,
     219111576,    829112632,    455614152,   1064192952,    313010856,
     820754920},                                                       
                                                                        
   {1073741820,     71582788,    603989028,        37500,       120660,
     182195932,    213921796,    473570692,     41763004,    156352828,
      88343188,    698012780,    666108868,    337608156,   1054329884,
     799472220,    641885244,    392104980,    502284340,   1002405628,
     249907140,     75586228,    206587660,    565275348,   1021426548,
     425987996,    598058580,    401940420,    919427316,    822049324,
     115655868,    759299588,    562394644,    990942164,   1003212268,
     598750292,    675334852,    675293340,    445665316,    903023756,
     872412692,    172640916,   1051615436,     91229620,     94586692,
     344873452,      7029156,    683421900,    434258804,    955002092,
     436424380},                                                       
                                                                        
   {1073741822,    644245094,   1040195102,    537039474,    537089354,
     642656334,     73402402,    664239174,   1014620426,    500917234,
    1042677826,    347788318,   1069154738,    373259762,    362587674,
     239395914,    132283950,    903121466,    445210638,    968851198,
     230153254,    935549622,    308815630,    861891286,    496995094,
     670740382,    657311830,    573565382,    248331478,   1064650798,
     338312798,    669059078,   1065190902,    379125622,    111897166,
     520650422,    740300390,   1046483950,     66254898,    992513134,
     234871606,    610553330,    450553866,    566758134,    787800806,
    1071709866,    971732606,    528102354,    790919122,    917384366,
     656244162},                                                       
                                                                        
   {1073741823,    357913941,     50344755,    268538457,    805540473,
       3487029,      3146769,    592038967,    729591963,    781578389,
      66781679,    113956361,    331153483,    967802327,    935343371,
     324351309,    609305915,    818137857,    131059769,    918519549,
     827341719,    922770101,    456972047,    363883221,   1057014661,
     900956063,    580478293,    520383687,    315470533,    227601711,
     169598765,    909227271,    796983815,    349496709,    393974911,
     378320037,    777004343,    927999269,    616377385,    345930959,
     989849787,    627400495,    892675125,    260314121,   1041964063,
     531367163,    195776757,    237309785,    949187605,    719002587,
     495745187},                                                       
};

static long   PSUADE_ACPO_MaskVect[PSUADE_ACPO_MaxDimension];
static double PSUADE_ACPO_scale=9.31322574615478516E-10;
                                                                                
// ************************************************************************
// Constructor
// ------------------------------------------------------------------------
LPtauSampling::LPtauSampling() : Sampling()
{
  samplingID_ = PSUADE_SAMP_LPTAU;
}

// *******************************************************************
// destructor 
// -------------------------------------------------------------------
LPtauSampling::~LPtauSampling()
{
}

// ************************************************************************
// initialize the sampling data
// ------------------------------------------------------------------------
int LPtauSampling::initialize(int initLevel)
{
  int    sampleID, inputID;
  double ddata, ddata2, perturb;
  psVector vecRanges;

  if (nSamples_ == 0)
  {
    printf("LPtauSampling::initialize ERROR - nSamples = 0.\n");
    exit(1);
  }
  if (nInputs_ == 0)
  {
    printf("LPtauSampling::initialize ERROR - input not set up.\n");
    exit(1);
  }

  if (nInputs_ > PSUADE_ACPO_MaxDimension)  
  {
    printf("LPtauSampling : nInputs must be < %d.\n",
           PSUADE_ACPO_MaxDimension);
    exit(1);
  }
  if (nSamples_ > PSUADE_ACPO_MaxNSamples)  
  {
    printf("LPtauSampling : nSamples too large.\n");
    exit(1);
  }
  if (initLevel != 0) return 0;
  allocSampleData();

  if (printLevel_ > 4)
  {
    printf("LPtauSampling::initialize: nSamples = %d\n", nSamples_);
    printf("LPtauSampling::initialize: nInputs  = %d\n", nInputs_);
    printf("LPtauSampling::initialize: nOutputs = %d\n", nOutputs_);
    for (inputID = 0; inputID < nInputs_; inputID++)
      printf("    LPtauSampling input %3d = [%e %e]\n", inputID+1,
             vecLBs_[inputID], vecUBs_[inputID]);
  }

  vecRanges.setLength(nInputs_);
  for (inputID = 0;  inputID < nInputs_;  inputID++) 
    vecRanges[inputID] = vecUBs_[inputID] - vecLBs_[inputID];

  perturb = 0.0;
  if (randomize_ != 0)
    perturb = 0.5 * pow(1.0/(double) nSamples_, 1.0/(double) nInputs_);
  double *dPtr = vecSamInps_.getDVector();
  for (sampleID = 0; sampleID < nSamples_; sampleID++)
  {
    genLPtau(sampleID, &(dPtr[sampleID*nInputs_]));
    for (inputID = 0; inputID < nInputs_; inputID++)
    {
      ddata = vecSamInps_[sampleID*nInputs_+inputID];
      if (PSUADE_drand() < 0.5) ddata2 = -1;
      else                      ddata2 = 1;
      ddata2 *= PSUADE_drand() * perturb;
      ddata += ddata2;
      if (ddata > 1.0 || ddata < 0.0) ddata -= 2.0 * ddata2;
      ddata = ddata * vecRanges[inputID] + vecLBs_[inputID];
      vecSamInps_[sampleID*nInputs_+inputID] = ddata;
    }
  }
  return 0;
}

// ************************************************************************
// refine the sample space
// ------------------------------------------------------------------------
int LPtauSampling::refine(int refineRatio, int randomize, double thresh,
                          int nSamples, double *sampleErrors)
{
  int    sampleID, inputID, outputID, nLevels;
  double ddata, ddata2, perturb;
  psVector  vecRanges, vecNewSamInps, vecNewSamOuts;
  psIVector vecNewSamStas;

  (void) thresh;
  (void) nSamples;
  (void) sampleErrors;

  nLevels = refineRatio;
  if (nSamples_*nLevels > PSUADE_ACPO_MaxNSamples)  
  {
    printf("LPtauSampling::refine ERROR - nSamples too large.\n");
    exit(1);
  } 
  else if (nSamples_*nLevels <= 0)
  {
    printf("LPtauSampling::refine ERROR - nSamples_*nLevels <= 0. \n");
    exit(1);
  }

  vecNewSamInps.setLength(nSamples_*nLevels*nInputs_);
  vecNewSamOuts.setLength(nSamples_*nLevels*nOutputs_);
  vecNewSamStas.setLength(nSamples_*nLevels);
  for (sampleID = 0;  sampleID < nSamples_*nLevels; sampleID++)
  {
    for (outputID = 0; outputID < nOutputs_; outputID++)
      vecNewSamOuts[sampleID*nOutputs_+outputID] = PSUADE_UNDEFINED;
  }

  for (sampleID = 0;  sampleID < nSamples_; sampleID++) 
  {
    for (inputID = 0; inputID < nInputs_; inputID++)
      vecNewSamInps[sampleID*nInputs_+inputID] = 
              vecSamInps_[sampleID*nSamples_+inputID];
    for (outputID = 0; outputID < nOutputs_; outputID++)
      vecNewSamOuts[sampleID*nOutputs_+outputID] = 
                  vecSamOuts_[sampleID*nOutputs_+outputID];
    vecNewSamStas[sampleID] = vecSamStas_[sampleID];
  }

  vecRanges.setLength(nInputs_);
  for (inputID = 0;  inputID < nInputs_;  inputID++) 
    vecRanges[inputID] = vecUBs_[inputID] - vecLBs_[inputID];

  double *dPtr;
  perturb = 0.0;
  if (randomize != 0)
    perturb = 0.5 * pow(1.0/(double) nSamples_, 1.0/(double) nInputs_);
  for (sampleID = 0; sampleID < nSamples_*nLevels; sampleID++)
  {
    if (sampleID < nSamples_)
    {
      dPtr = vecNewSamInps.getDVector();
      dPtr = &(dPtr[nSamples_*nInputs_]);
      genLPtau(sampleID, dPtr);
    }
    else
    {
      dPtr = vecNewSamInps.getDVector();
      dPtr = &(dPtr[sampleID*nInputs_]);
      genLPtau(sampleID, dPtr);
      for (inputID = 0; inputID < nInputs_; inputID++)
      {
        ddata = vecNewSamInps[sampleID*nInputs_+inputID];
        if (PSUADE_drand() < 0.5) ddata2 = -1;
        else                      ddata2 = 1;
        ddata2 *= PSUADE_drand() * perturb;
        ddata += ddata2;
        if (ddata > 1.0 || ddata < 0.0) ddata -= 2.0 * ddata2;
        ddata = ddata * vecRanges[inputID] + vecLBs_[inputID];
        vecNewSamInps[sampleID*nInputs_+inputID] = ddata;
      }
    }
  }

  nSamples_ = nSamples_ * nLevels;
  vecSamInps_ = vecNewSamInps;
  vecSamOuts_ = vecNewSamOuts;
  vecSamStas_ = vecNewSamStas;

  if (printLevel_ > 4)
  {
    printf("LPtauSampling::refine: nSamples = %d\n", nSamples_);
    printf("LPtauSampling::refine: nInputs  = %d\n", nInputs_);
    printf("LPtauSampling::refine: nOutputs = %d\n", nOutputs_);
    for (inputID = 0; inputID < nInputs_; inputID++)
      printf("    LPtauSampling input %3d = [%e %e]\n", inputID+1,
             vecLBs_[inputID], vecUBs_[inputID]);
  }
  return 0;
}

// ************************************************************************
// generate an LPtau sample 
// reference : B. Shukhman (GENERATION OF QUASI-RANDOM (LPTAU) VECTORS FOR
//             PARALLEL COMPUTATION.  B. SHUKHMAN. . IN COMP. PHYS. COMMUN.
//             78 (1994) 279 (the ACPO code)
// ------------------------------------------------------------------------
int LPtauSampling::genLPtau(int sampleID, double *sampleInputs)
{
  int  j,k;
  long BitString;
                                                                                
  if (sampleID == 0)
  {
    for (j = 0; j < nInputs_; j++) PSUADE_ACPO_MaskVect[j] = 0;
    for (k=0,BitString=sampleID^sampleID>>1;BitString;BitString>>=1,k++)
    {
      if (BitString & 1)
        for (j = 0; j < nInputs_; j++) 
          PSUADE_ACPO_MaskVect[j] ^= PSUADE_ACPO_Directing[k][j];
    }
    for (j = 0; j < nInputs_; j++)
      sampleInputs[j] = PSUADE_ACPO_MaskVect[j] * PSUADE_ACPO_scale;
  }
  else /* recurrent generation of the sample points */
  {
    for (k=0, BitString=sampleID;(BitString & 1)==0; k++, BitString>>=1);
    for (j = 0; j < nInputs_; j++)
    {
      PSUADE_ACPO_MaskVect[j] ^= PSUADE_ACPO_Directing[k][j];
      sampleInputs[j] = PSUADE_ACPO_MaskVect[j] * PSUADE_ACPO_scale;
    }
  }
  return 0;
}

// ************************************************************************
// equal operator
// ------------------------------------------------------------------------
LPtauSampling& LPtauSampling::operator=(const LPtauSampling &)
{
  printf("LPtauSampling operator= ERROR: operation not allowed.\n");
  exit(1);
  return (*this);
}

