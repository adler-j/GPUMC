 
    >>>> SAMPLE SIMULATION FOR MC-GPU v1.3 USING THE ZUBAL PHANTOM >>>>

    
                      @file    README_sample_simulation_Zubal_phantom.txt
                      @author  Andreu Badal (Andreu.Badal-Soler{at}fda.hhs.gov)
                      @date    2012/12/12

    
The folder "SAMPLE_SIMULATION_Zubal_phantom" contains the MC-GPU v1.3 input files required  
to run a sample CT scan simulation with a voxelized human phantom.


  -- GEOMETRY:

The voxelized geometry of the Zubal phantom developed at Yale is not provided here but it can 
be freely obtained online at the address http://noodle.med.yale.edu/zubal/.

A utility program called "zubal2mcgpu" is provided to convert the original Zubal phantom file 
into the format used by MC-GPU (and penEasy). After downloading the binary file "voxel_man.dat" 
from the website, the MC-GPU geometry can be obtained with the following commands:

  $ gcc -O3 zubal2mcgpu.c -o zubal2mcgpu.x   # (Code compilation)
  $ ./zubal2mcgpu.x zubal2mcgpu_conversion_table.in voxel_man.dat  # (Format conversion)
  $ gzip voxel_man.dat.vox   # (Optional compression of the phantom to save space on disk)

The output file "voxel_man.dat.vox.gz" can then be directly used with MC-GPU.
Remember that if you use the Zubal phantom in your work you should cite the following publication:
   Zubal, I.G., Harrell, C.R, Smith, E.O, Rattner, Z., Gindi, G. and Hoffer, P.B., Computerized 
   three-dimensional segmented human anatomy, Medical Physics, 21(2):299-302, 1994.

The example conversion table provided with zubal2mcgpu distributes the phantom's organs among 15 
different organic materials. The assignments from organ to material should be reviewed and possibly 
modified by the users depending on the particular simulation to be run. This table and software can
be easily adapted to import other binary phantoms into the MC-GPU format.



  -- SIMULATION PARAMETERS:

The input file "MC-GPU_v1.3_Zubal.in" simulates a very basic CT geometry intended to 
show how to obtain three projection images acquired around the phantom. 

To run the simulation from the Linux command line:
  $ cd SAMPLE_SIMULATION_Zubal_phantom
  $ ../MC-GPU_v1.3.x MC-GPU_v1.3_Zubal.in | tee MC-GPU_v1.3_Zubal.out
   
To visualize the output images:
  $ gnuplot ../GNUPLOT_SCRIPTS_VISUALIZATION/gnuplot_images_MC-GPU_CT.gpl

The default simulation parameters given in the input file are the following:

#[SECTION SIMULATION CONFIG v.2009-05-12]
1.0e8                           # TOTAL NUMBER OF HISTORIES, OR SIMULATION TIME IN SECONDS IF VALUE < 100000
1234567890                      # RANDOM SEED (ranecu PRNG)
0                               # GPU NUMBER TO USE WHEN MPI IS NOT USED, OR TO BE AVOIDED IN MPI RUNS
128                             # GPU THREADS PER CUDA BLOCK (multiple of 32)
150                             # SIMULATED HISTORIES PER GPU THREAD

#[SECTION SOURCE v.2011-07-12]
../90kVp_4.0mmAl.spc            # X-RAY ENERGY SPECTRUM FILE
25.6 -37.0  65.0                # SOURCE POSITION: X Y Z [cm]
 0.0   1.0   0.0                # SOURCE DIRECTION COSINES: U V W
-15.0 -15.0                     # POLAR AND AZIMUTHAL APERTURES FOR THE FAN BEAM [degrees] (input negative to cover the whole detector)

#[SECTION IMAGE DETECTOR v.2009-12-02]
mc-gpu_image.dat                # OUTPUT IMAGE FILE NAME
350    250                      # NUMBER OF PIXELS IN THE IMAGE: Nx Nz
 70.0   50.0                    # IMAGE SIZE (width, height): Dx Dz [cm]
 90.0                           # SOURCE-TO-DETECTOR DISTANCE (detector set in front of the source, perpendicular to the initial direction)

#[SECTION CT SCAN TRAJECTORY v.2011-10-25]
3                               # NUMBER OF PROJECTIONS (beam must be perpendicular to Z axis, set to 1 for a single projection)
45.0                            # ANGLE BETWEEN PROJECTIONS [degrees] (360/num_projections for full CT)
 0.0 5000.0                     # ANGLES OF INTEREST (projections outside the input interval will be skipped)
60.0                            # SOURCE-TO-ROTATION AXIS DISTANCE (rotation radius, axis parallel to Z)
 0.0                            # VERTICAL TRANSLATION BETWEEN PROJECTIONS (HELICAL SCAN)

#[SECTION DOSE DEPOSITION v.2012-12-12]
YES                             # TALLY MATERIAL DOSE? [YES/NO] (electrons not transported, x-ray energy locally deposited at interaction)
YES                             # TALLY 3D VOXEL DOSE? [YES/NO] (dose measured separately for each voxel)
mc-gpu_dose.dat                 # OUTPUT VOXEL DOSE FILE NAME
1  128                          # VOXEL DOSE ROI: X-index min max (first voxel has index 1)
1  128                          # VOXEL DOSE ROI: Y-index min max
1  243                          # VOXEL DOSE ROI: Z-index min max
 
#[SECTION VOXELIZED GEOMETRY FILE v.2009-11-30]
Zubal_voxel_man.vox.gz          # VOXEL GEOMETRY FILE (penEasy 2008 format; .gz accepted)

#[SECTION MATERIAL FILE LIST v.2009-11-30]
../MC-GPU_material_files/air__5-120keV.mcgpu.gz                     #  1st MATERIAL FILE (.gz accepted)
../MC-GPU_material_files/muscle_ICRP110__5-120keV.mcgpu.gz          #  2nd MATERIAL FILE
../MC-GPU_material_files/soft_tissue_ICRP110__5-120keV.mcgpu.gz     #  3rd MATERIAL FILE
../MC-GPU_material_files/bone_ICRP110__5-120keV.mcgpu.gz            #  4th MATERIAL FILE
../MC-GPU_material_files/cartilage_ICRP110__5-120keV.mcgpu.gz       #  5th MATERIAL FILE
../MC-GPU_material_files/adipose_ICRP110__5-120keV.mcgpu.gz         #  6th MATERIAL FILE
../MC-GPU_material_files/blood_ICRP110__5-120keV.mcgpu.gz           #  7th MATERIAL FILE
../MC-GPU_material_files/skin_ICRP110__5-120keV.mcgpu.gz            #  8th MATERIAL FILE
../MC-GPU_material_files/lung_ICRP110__5-120keV.mcgpu.gz            #  9th MATERIAL FILE
../MC-GPU_material_files/glands_others_ICRP110__5-120keV.mcgpu.gz   # 10th MATERIAL FILE
../MC-GPU_material_files/brain_ICRP110__5-120keV.mcgpu.gz           # 11th MATERIAL FILE
../MC-GPU_material_files/red_marrow_Woodard__5-120keV.mcgpu.gz      # 12th MATERIAL FILE
../MC-GPU_material_files/liver_ICRP110__5-120keV.mcgpu.gz           # 13th MATERIAL FILE
../MC-GPU_material_files/stomach_intestines_ICRP110__5-120keV.mcgpu.gz   #  14th MATERIAL FILE
../MC-GPU_material_files/water__5-120keV.mcgpu.gz                   #  15th MATERIAL FILE

