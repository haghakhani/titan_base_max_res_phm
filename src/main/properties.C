/*******************************************************************
 * Copyright (C) 2003 University at Buffalo
 *
 * This software can be redistributed free of charge.  See COPYING
 * file in the top distribution directory for more details.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Author: 
 * Description: 
 *
 *******************************************************************
 * $Id: properties.h 233 2012-03-27 18:30:40Z dkumar $ 
 */



#include "../header/properties.h"


PileProps::PileProps()
{
    numpiles = 0;
    length_scale = 1.0;
    height_scale = 1.0;
    velocity_scale = 1.0;
}
PileProps::~PileProps()
{
}
//! function allocates space for the pile data
void PileProps::allocpiles(int numpiles_in)
{
    numpiles = numpiles_in;
    pileheight.resize(numpiles);

    xCen.resize(numpiles);
    yCen.resize(numpiles);
    majorrad.resize(numpiles);
    minorrad.resize(numpiles);
    cosrot.resize(numpiles);
    sinrot.resize(numpiles);
    initialVx.resize(numpiles);
    initialVy.resize(numpiles);
}

void PileProps::addPile(double hight, double xcenter, double ycenter, double majradius, double minradius,
                     double orientation, double Vmagnitude, double Vdirection)
{
    numpiles++;
    pileheight.push_back(hight);
    xCen.push_back(xcenter);
    yCen.push_back(ycenter);
    majorrad.push_back(majradius);
    minorrad.push_back(minradius);
    cosrot.push_back(cos(orientation * PI / 180.0));
    sinrot.push_back(sin(orientation * PI / 180.0));
    initialVx.push_back(Vmagnitude * cos(Vdirection * PI / 180.0));
    initialVy.push_back(Vmagnitude * sin(Vdirection * PI / 180.0));
}

void PileProps::scale(double m_length_scale,double m_height_scale,double m_gravity_scale)
{
    length_scale=m_length_scale;
    height_scale=m_height_scale;
    //non-dimensionalize the inputs
    velocity_scale = sqrt(length_scale * m_gravity_scale);
    int isrc;
    for(isrc = 0; isrc < numpiles; isrc++)
    {
        pileheight[isrc] /= height_scale;
        xCen[isrc] /= length_scale;
        yCen[isrc] /= length_scale;
        majorrad[isrc] /= length_scale;
        minorrad[isrc] /= length_scale;
        initialVx[isrc] /= velocity_scale;
        initialVy[isrc] /= velocity_scale;
    }
}
double PileProps::get_smallest_pile_radius()
{
    double smallestpileradius = HUGE_VAL;
    int isrc;
    for(isrc = 0; isrc < numpiles; isrc++)
    {
        if(smallestpileradius > majorrad[isrc])
            smallestpileradius = majorrad[isrc];

        if(smallestpileradius > minorrad[isrc])
            smallestpileradius = minorrad[isrc];
    }
    return smallestpileradius;
}
void PileProps::print_pile(int i)
{
    printf("\tPile %d\n", i);
    printf("\t\tMaximum Initial Thickness, P (m):%f\n", pileheight[i]*height_scale);
    printf("\t\tCenter of Initial Volume, xc, yc (UTM E, UTM N): %f %f\n", xCen[i]*length_scale, yCen[i]*length_scale);
    printf("\t\tMajor and Minor Extent, majorR, minorR (m, m): %f %f\n", majorrad[i]*length_scale, minorrad[i]*length_scale);
    double orientation=atan2(sinrot[i],cosrot[i])*180.0/PI;
    printf("\t\tOrientation (angle [degrees] from X axis to major axis): %f\n", orientation);
    double Vmagnitude=sqrt(initialVx[i]*initialVx[i]+initialVy[i]*initialVy[i]);
    double Vdirection=atan2(initialVy[i],initialVx[i])*180.0/PI;
    printf("\t\tInitial speed [m/s]: %f\n", Vmagnitude*velocity_scale);
    printf("\t\tInitial direction ([degrees] from X axis): %f\n", Vdirection);
    printf("\t\tPile volume [m^3]: %f\n", get_volume(i)*height_scale*length_scale*length_scale);
}
void PileProps::print0()
{
    int i;
    if(numpiles>0)
    {
        printf("Piles:    (Number of piles: %d)\n", numpiles);
        for(i = 0; i < numpiles; i++)
            print_pile(i);
    }
    else
    {
        printf("Piles:    there is no piles\n");
    }
}



PilePropsTwoPhases::PilePropsTwoPhases() :
            PileProps()
{
}
PilePropsTwoPhases::~PilePropsTwoPhases()
{
}

void PilePropsTwoPhases::allocpiles(int numpiles_in)
{
    PileProps::allocpiles(numpiles_in);
    vol_fract.resize(numpiles);
}
void PilePropsTwoPhases::addPile(double hight, double xcenter, double ycenter, double majradius, double minradius,
                     double orientation, double Vmagnitude, double Vdirection)
{
    addPile(hight, xcenter, ycenter, majradius, minradius, orientation, Vmagnitude, Vdirection, 1.0);
}
void PilePropsTwoPhases::addPile(double hight, double xcenter, double ycenter, double majradius, double minradius,
                     double orientation, double Vmagnitude, double Vdirection, double volfract)
{
    PileProps::addPile(hight, xcenter, ycenter, majradius, minradius, orientation, Vmagnitude, Vdirection);
    vol_fract.push_back(volfract);
}
void PilePropsTwoPhases::print_pile(int i)
{
    PileProps::print_pile(i);
    printf("\t\tInitial solid-volume fraction,(0:1.): %f\n", vol_fract[i]);
}