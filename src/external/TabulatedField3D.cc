//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// Code developed by:
//  S.Larsson and J. Generowicz.
//
//    *************************************
//    *                                   *
//    *    PurgMagTabulatedField3D.cc     *
//    *                                   *
//    *************************************
//
// $Id: PurgMagTabulatedField3D.cc,v 1.4 2006-06-29 16:06:25 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "TabulatedField3D.hh"

TabulatedField3D::TabulatedField3D( const char* filename, double zOffset ) 
  :fZoffset(zOffset),invertX(false),invertY(false),invertZ(false)
{    
 
  double lenUnit= millimeter;
  double fieldUnit= kilovolt/cm; 
  G4cout << "\n-----------------------------------------------------------"
	 << "\n      Electric field"
	 << "\n-----------------------------------------------------------";
    
  G4cout << "\n ---> " "Reading the field grid from " << filename << " ... " << endl; 
  ifstream file( filename ); // Open the file for reading.
  
  // Ignore first blank line
  char buffer[256];
  file.getline(buffer,256);
  
  // Read table dimensions 
  file >> nx >> ny >> nz; // Note dodgy order

  G4cout << "  [ Number of values x,y,z: " 
	 << nx << " " << ny << " " << nz << " ] "
	 << endl;

  // Set up storage space for table
  xField.resize( nx );
  yField.resize( nx );
  zField.resize( nx );
  int ix, iy, iz;
  for (ix=0; ix<nx; ix++) {
    xField[ix].resize(ny);
    yField[ix].resize(ny);
    zField[ix].resize(ny);
    for (iy=0; iy<ny; iy++) {
      xField[ix][iy].resize(nz);
      yField[ix][iy].resize(nz);
      zField[ix][iy].resize(nz);
    }
  }
  
  // Ignore other header information    
  // The first line whose second character is '0' is considered to
  // be the last line of the header.
  do {
    file.getline(buffer,256);
  } while ( buffer[1]!='0');
  
  // Read in the data
  double xval,yval,zval,ex,ey,ez;
  //  double permeability; // Not used in this example.
  for (ix=0; ix<nx; ix++) {
    for (iy=0; iy<ny; iy++) {
      for (iz=0; iz<nz; iz++) {
        file >> xval >> yval >> zval >> ex >> ey >> ez;// >> permeability;
        if ( ix==0 && iy==0 && iz==0 ) {
          minx = xval * lenUnit;
          miny = yval * lenUnit;
          minz = zval * lenUnit;
        }
        xField[ix][iy][iz] = ex * fieldUnit;
        yField[ix][iy][iz] = ey * fieldUnit;
        zField[ix][iy][iz] = ez * fieldUnit;
      }
    }
  }
  file.close();

  maxx = xval * lenUnit;
  maxy = yval * lenUnit;
  maxz = zval * lenUnit;

  G4cout << "\n ---> ... done reading " << endl;

  // G4cout << " Read values of field from file " << filename << endl; 
  G4cout << " ---> assumed the order:  x, y, z, Ex, Ey, Ez "
	 << "\n ---> Min values x,y,z: " 
	 << minx/cm << " " << miny/cm << " " << minz/cm << " cm "
	 << "\n ---> Max values x,y,z: " 
	 << maxx/cm << " " << maxy/cm << " " << maxz/cm << " cm "
	 << "\n ---> The field will be offset ey " << zOffset/cm << " cm " << endl;

  // Should really check that the limits are not the wrong way around.
  if (maxx < minx) {swap(maxx,minx); invertX = true;} 
  if (maxy < miny) {swap(maxy,miny); invertY = true;} 
  if (maxz < minz) {swap(maxz,minz); invertZ = true;} 
  G4cout << "\nAfter reordering if neccesary"  
	 << "\n ---> Min values x,y,z: " 
	 << minx/cm << " " << miny/cm << " " << minz/cm << " cm "
	 << " \n ---> Max values x,y,z: " 
	 << maxx/cm << " " << maxy/cm << " " << maxz/cm << " cm ";

  dx = maxx - minx;
  dy = maxy - miny;
  dz = maxz - minz;
  G4cout << "\n ---> Dif values x,y,z (range): " 
	 << dx/cm << " " << dy/cm << " " << dz/cm << " cm in z "
	 << "\n-----------------------------------------------------------" << endl;
}

void TabulatedField3D::GetFieldValue(const double point[4],
				      double *Efield ) const
{

  double x = point[0];
  double y = point[1];
  double z = point[2] + fZoffset;

  // Ey  is defined fore y > 0 but assumed perfectly symmetric
  y = fabs(y);

  // Check that the point is within the defined region 
  if ( x>=minx && x<=maxx &&
       y>=miny && y<=maxy && 
       z>=minz && z<=maxz ) {
    
    // Position of given point within region, normalized to the range
    // [0,1]
    double xfraction = (x - minx) / dx;
    double yfraction = (y - miny) / dy; 
    double zfraction = (z - minz) / dz;

    if (invertX) { xfraction = 1 - xfraction;}
    if (invertY) { yfraction = 1 - yfraction;}
    if (invertZ) { zfraction = 1 - zfraction;}

    // Need addresses of these to pass to modf below.
    // modf uses its second argument as an OUTPUT argument.
    double xdindex, ydindex, zdindex;
    
    // Position of the point within the cuboid defined by the
    // nearest surrounding tabulated points
    double xlocal = ( std::modf(xfraction*(nx-1), &xdindex));
    double ylocal = ( std::modf(yfraction*(ny-1), &ydindex));
    double zlocal = ( std::modf(zfraction*(nz-1), &zdindex));
    
    // The indices of the nearest tabulated point whose coordinates
    // are all less than those of the given point
    int xindex = static_cast<int>(xdindex);
    int yindex = static_cast<int>(ydindex);
    int zindex = static_cast<int>(zdindex);
    

#ifdef DEBUG_INTERPOLATING_FIELD
    G4cout << "Local x,y,z: " << xlocal << " " << ylocal << " " << zlocal << endl;
    G4cout << "Index x,y,z: " << xindex << " " << yindex << " " << zindex << endl;
    double valx0z0, mulx0z0, valx1z0, mulx1z0;
    double valx0z1, mulx0z1, valx1z1, mulx1z1;
    valx0z0= table[xindex  ][0][zindex];  mulx0z0=  (1-xlocal) * (1-zlocal);
    valx1z0= table[xindex+1][0][zindex];  mulx1z0=   xlocal    * (1-zlocal);
    valx0z1= table[xindex  ][0][zindex+1]; mulx0z1= (1-xlocal) * zlocal;
    valx1z1= table[xindex+1][0][zindex+1]; mulx1z1=  xlocal    * zlocal;
#endif

        // Full 3-dimensional version
    Efield[0] =
      xField[xindex  ][yindex  ][zindex  ] * (1-xlocal) * (1-ylocal) * (1-zlocal) +
      xField[xindex  ][yindex  ][zindex+1] * (1-xlocal) * (1-ylocal) *    zlocal  +
      xField[xindex  ][yindex+1][zindex  ] * (1-xlocal) *    ylocal  * (1-zlocal) +
      xField[xindex  ][yindex+1][zindex+1] * (1-xlocal) *    ylocal  *    zlocal  +
      xField[xindex+1][yindex  ][zindex  ] *    xlocal  * (1-ylocal) * (1-zlocal) +
      xField[xindex+1][yindex  ][zindex+1] *    xlocal  * (1-ylocal) *    zlocal  +
      xField[xindex+1][yindex+1][zindex  ] *    xlocal  *    ylocal  * (1-zlocal) +
      xField[xindex+1][yindex+1][zindex+1] *    xlocal  *    ylocal  *    zlocal ;
    Efield[1] =
      yField[xindex  ][yindex  ][zindex  ] * (1-xlocal) * (1-ylocal) * (1-zlocal) +
      yField[xindex  ][yindex  ][zindex+1] * (1-xlocal) * (1-ylocal) *    zlocal  +
      yField[xindex  ][yindex+1][zindex  ] * (1-xlocal) *    ylocal  * (1-zlocal) +
      yField[xindex  ][yindex+1][zindex+1] * (1-xlocal) *    ylocal  *    zlocal  +
      yField[xindex+1][yindex  ][zindex  ] *    xlocal  * (1-ylocal) * (1-zlocal) +
      yField[xindex+1][yindex  ][zindex+1] *    xlocal  * (1-ylocal) *    zlocal  +
      yField[xindex+1][yindex+1][zindex  ] *    xlocal  *    ylocal  * (1-zlocal) +
      yField[xindex+1][yindex+1][zindex+1] *    xlocal  *    ylocal  *    zlocal ;
    Efield[2] =
      zField[xindex  ][yindex  ][zindex  ] * (1-xlocal) * (1-ylocal) * (1-zlocal) +
      zField[xindex  ][yindex  ][zindex+1] * (1-xlocal) * (1-ylocal) *    zlocal  +
      zField[xindex  ][yindex+1][zindex  ] * (1-xlocal) *    ylocal  * (1-zlocal) +
      zField[xindex  ][yindex+1][zindex+1] * (1-xlocal) *    ylocal  *    zlocal  +
      zField[xindex+1][yindex  ][zindex  ] *    xlocal  * (1-ylocal) * (1-zlocal) +
      zField[xindex+1][yindex  ][zindex+1] *    xlocal  * (1-ylocal) *    zlocal  +
      zField[xindex+1][yindex+1][zindex  ] *    xlocal  *    ylocal  * (1-zlocal) +
      zField[xindex+1][yindex+1][zindex+1] *    xlocal  *    ylocal  *    zlocal ;

  } else {
    Efield[0] = 0.0;
    Efield[1] = 0.0;
    Efield[2] = 0.0;
  }
  // G4cout << "Get field value at " << x/mm << ", " << y/mm << ", " << z/mm
  //      << G4endl;
  // G4cout << Efield[0]/(kilovolt/cm) << ", " << Efield[1]/(kilovolt/cm) << ", "
  //        << Efield[2]/(kilovolt/cm) << G4endl;
  // int j;
  // cin >> j;
}

