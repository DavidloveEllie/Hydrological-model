#include <stdio.h>
#include <stdlib.h>
#include <vicNl.h>

static char vcid[] = "$Id: initialize_soil.c,v 4.3.2.5 2011/03/07 05:14:57 vicadmin Exp $";

void initialize_soil (cell_data_struct **cell, 
                      soil_con_struct   *soil_con,
                      veg_con_struct    *veg_con,
		      int                veg_num)
/**********************************************************************
	initialize_soil		Keith Cherkauer		July 31, 1996

  This routine initializes the soil variable arrays for each new
  grid cell.

  modifications:
  11-18-02 Modified to initialize wetland soil moisture.          LCB
  2006-Nov-07 Removed LAKE_MODEL option.				TJB
  2007-Aug-10 Added features for EXCESS_ICE option.			JCA
  2009-Mar-16 Modified to use min_liq (minimum allowable liquid water
	      content) instead of resid_moist.  For unfrozen soil,
	      min_liq = resid_moist.					TJB
  2009-Jul-31 Replaced extra lake/wetland veg tile with reference to
	      veg_con[j].LAKE.						TJB
  2009-Dec-11 Removed min_liq and options.MIN_LIQ.			TJB
  2011-Mar-01 Now initializes more cell data structure terms, including
	      asat and zwt.						TJB
**********************************************************************/
{
  extern option_struct options;

  int veg, band, lindex, frost_area;
  double tmp_moist[MAX_LAYERS];
  double tmp_runoff;
  
  for ( veg = 0 ; veg <= veg_num ; veg++) {
    for(band=0;band<options.SNOW_BAND;band++) {
      cell[veg][band].baseflow = 0;
      cell[veg][band].runoff = 0;
      for(lindex=0;lindex<options.Nlayer;lindex++) {
	cell[veg][band].layer[lindex].evap = 0;
	cell[veg][band].layer[lindex].moist = soil_con->init_moist[lindex];
        if (cell[veg][band].layer[lindex].moist > soil_con->max_moist[lindex]) cell[veg][band].layer[lindex].moist = soil_con->max_moist[ lindex];
        tmp_moist[lindex] = cell[veg][band].layer[lindex].moist;
#if SPATIAL_FROST
        for (frost_area=0; frost_area<FROST_SUBAREAS; frost_area++) {
          cell[veg][band].layer[lindex].ice[frost_area] = 0;
        }
#else
        cell[veg][band].layer[lindex].ice = 0;
#endif
      }
      compute_runoff_and_asat(soil_con, tmp_moist, 0, &(cell[veg][band].asat), &tmp_runoff);
      wrap_compute_zwt(soil_con, &(cell[veg][band]));
    }
  }

}
