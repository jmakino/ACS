              // Time-stamp: <2001-12-14 20:05:58 piet>
             //================================================================
            //                                                                |
           //           /__----__                         ........            |
          //       .           \                     ....:        :.          |
         //       :                 _\|/_         ..:                         |
        //       :                   /|\         :                     _\|/_  |
       //  ___   ___                  _____                      ___    /|\   |
      //  /     |   \    /\ \     / |   |   \  / |        /\    |   \         |
     //  |   __ |___/   /  \ \   /  |   |    \/  |       /  \   |___/         |
    //   |    | |  \   /____\ \ /   |   |    /   |      /____\  |   \     \/  |
   //     \___| |   \ /      \ V    |   |   /    |____ /      \ |___/     |   |
  //                                                                      /   |
 //              :                       _/|     :..                    |/    |
//                :..               ____/           :....          ..         |
/*   o   //          :.    _\|/_     /                   :........:           |
 *  O  `//\                 /|\                                               |
 *  |     /\                                                                  |
 *=============================================================================
 *
 *  grav.h:  generic header file for use in gravitylab.
 *_____________________________________________________________________________
 *
 *    version 1:  Dec 2001   Piet Hut
 *_____________________________________________________________________________
 */

#ifndef  GRAVITYLAB_GRAV_H               // to avoid multiple inclusion
#  define  GRAVITYLAB_GRAV_H             // of this header file, from here...

#include  <iostream>
#include  <cmath>                        // to include sqrt(), etc.
using namespace std;

typedef double  real;                    // "real" as a general name for the
                                         // standard floating-point data type

const int NDIM = 3;                      // number of spatial dimensions

#endif                                   // ...till here, to avoid multiple
                                         // inclusion of this header file

/*-----------------------------------------------------------------------------
 *                                                                    \\   o
 *  end of file:  grav.h                                              /\\'  O
 *                                                                   /\     |
 *=============================================================================
 */
