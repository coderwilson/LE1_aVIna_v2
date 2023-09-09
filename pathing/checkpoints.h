#pragma once
#include "../config/global_objects.h"


float* get_checkpoint(int checkpoint = 0) {
    float retval[2] = { 0, 0 };
    if (main_vars.gamestate == 10) {
        // Eden Prime, up to Ashley
        int checkpoint_coordiantes[28][2] = {
            { -23036, 121187} ,
            {-22802, 121525} ,
            {-21607, 123396} ,
            {-21193, 123597} ,
            {-19852, 123507} ,
            {-18239, 123826} ,
            {-17684, 124277} ,
            {-17006, 126197} ,  // Just before Jenkins scene
            {-16736, 126448},
            {-16742, 127270} ,
            {-16848, 128484} ,
            {-16718, 129389} ,
            {-16748, 130393} ,
            {-16568, 130995} ,
            {-16934, 132424} ,
            {-17469, 133299} ,
            {-18233, 133914} ,
            {-18568, 134787} ,
            {-18695, 135572} ,
            {-19471, 136133} ,
            {-19705, 138092} ,
            {-21350, 140290} ,
            {-22349, 140413} ,
            {-23521, 140923} ,  // Around the rock
            {-23521, 140923} ,
            {-23521, 140923} ,
            {-24796, 141254} ,
            {-26206, 140872}
        };
        retval[0] = (float)checkpoint_coordiantes[checkpoint][0];
        retval[1] = (float)checkpoint_coordiantes[checkpoint][1];
	}
    
    return retval;
}