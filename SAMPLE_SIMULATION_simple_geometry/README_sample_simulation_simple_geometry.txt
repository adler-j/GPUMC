 
    >>>> SAMPLE SIMULATION FOR MC-GPU v1.3 USING A SIMPLE GEOMETRY >>>>

    
                      @file    README_sample_simulation_simple_geometry.txt
                      @author  Andreu Badal (Andreu.Badal-Soler{at}fda.hhs.gov)
                      @date    2012/12/12

    
The folder "SAMPLE_SIMULATION_simple_geometry" contains the MC-GPU v1.3 input files required  
to run a very siple simulation with a geometry composed of just six voxels and a monoenergetic
90 keV x-ray source.


  -- GEOMETRY:

This example uses the simplest voxelized geometry: just six voxels with 2 materials 
and 3 different densities. Ther voxels are organized in two rows and 3 columns.
This file can be used as an example to generate geometry files compatible with MC-GPU 
and penEasy 2008.

    [SECTION VOXELS HEADER v.2008-04-13]
      3     1     2     No. OF VOXELS IN X,Y,Z
    10.0  10.0 15.0   VOXEL SIZE (cm) ALONG X,Y,Z
    1                  COLUMN NUMBER WHERE MATERIAL ID IS LOCATED
    2                  COLUMN NUMBER WHERE THE MASS DENSITY IS LOCATED
    1                  BLANK LINES AT END OF X,Y-CYCLES (1=YES,0=NO)
    [END OF VXH SECTION]
    1 1.50
    1 1.00
    1 0.50


    2 1.50
    2 1.00
    2 0.50



  -- SIMULATION PARAMETERS:

To run the simulation from the Linux command line:
  $ cd SAMPLE_SIMULATION_simple_geometry
  $ ../MC-GPU_v1.3.x MC-GPU_v1.3_6voxels.in | tee MC-GPU_v1.3_6voxels.out
   
To visualize the output image:
  $ gnuplot ../GNUPLOT_SCRIPTS_VISUALIZATION/gnuplot_images_MC-GPU.gpl

The default simulation parameters given in the input file are the following:


#[SECTION SIMULATION CONFIG v.2009-05-12]
1.0e8                           # TOTAL NUMBER OF HISTORIES, OR SIMULATION TIME IN SECONDS IF VALUE < 100000
271828182                       # RANDOM SEED (ranecu PRNG)
0                               # GPU NUMBER TO USE WHEN MPI IS NOT USED, OR TO BE AVOIDED IN MPI RUNS
128                             # GPU THREADS PER CUDA BLOCK (multiple of 32)
100                             # SIMULATED HISTORIES PER GPU THREAD

#[SECTION SOURCE v.2011-07-12]
90keV.spc                       # X-RAY ENERGY SPECTRUM FILE
15.0 -45.0  15.0                # SOURCE POSITION: X Y Z [cm]
0.0    1.0   0.0                # SOURCE DIRECTION COSINES: U V W
42.0  39.0                      # POLAR AND AZIMUTHAL APERTURES FOR THE FAN BEAM [degrees] (input negative to automatically cover the whole detector)

#[SECTION IMAGE DETECTOR v.2009-12-02]
mc-gpu_image.dat                # OUTPUT IMAGE FILE NAME
300    300                      # NUMBER OF PIXELS IN THE IMAGE: Nx Nz
 90.0   90.0                    # IMAGE SIZE (width, height): Dx Dz [cm]
100.0                           # SOURCE-TO-DETECTOR DISTANCE (detector set in front of the source, perpendicular to the initial direction)

#[SECTION CT SCAN TRAJECTORY v.2011-10-25]
1                               # NUMBER OF PROJECTIONS (beam must be perpendicular to Z axis, set to 1 for a single projection)
1.0                             # ANGLE BETWEEN PROJECTIONS [degrees] (360/num_projections for full CT)
0.0  3600.0                     # ANGLES OF INTEREST (projections outside the input interval will be skipped)
60.0                            # SOURCE-TO-ROTATION AXIS DISTANCE (rotation radius, axis parallel to Z)
 0.0                            # VERTICAL TRANSLATION BETWEEN PROJECTIONS (HELICAL SCAN)

#[SECTION DOSE DEPOSITION v.2012-12-12]
YES                             # TALLY MATERIAL DOSE? [YES/NO] (electrons not transported, x-ray energy locally deposited at interaction)
YES                             # TALLY 3D VOXEL DOSE? [YES/NO] (dose measured separately for each voxel)
mc-gpu_dose.dat                 # OUTPUT VOXEL DOSE FILE NAME
1  3                            # VOXEL DOSE ROI: X-index min max (first voxel has index 1)
1  1                            # VOXEL DOSE ROI: Y-index min max
1  2                            # VOXEL DOSE ROI: Z-index min max
 
#[SECTION VOXELIZED GEOMETRY FILE v.2009-11-30]
6voxels.vox                     # VOXEL GEOMETRY FILE (penEasy 2008 format; .gz accepted)

#[SECTION MATERIAL FILE LIST v.2009-11-30]
../MC-GPU_material_files/water__5-120keV.mcgpu.gz          #  1st MATERIAL FILE (.gz accepted)
../MC-GPU_material_files/bone_ICRP110__5-120keV.mcgpu.gz   #  2nd MATERIAL FILE
