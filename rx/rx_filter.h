/*
--------------------------------------------------------------------------------
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.
You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the
Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
Boston, MA  02110-1301, USA.
--------------------------------------------------------------------------------
*/

// Copyright (c) 2023 John Seamons, ZL/KF6VO

#pragma once

CFastFIR m_PassbandFIR[MAX_RX_CHANS];
CFastFIR m_chan_null_FIR[MAX_RX_CHANS];

CFir m_AM_FIR[MAX_RX_CHANS];

CFir m_nfm_deemp_FIR[MAX_RX_CHANS];     // see: tools/FIR.m
CFir m_am_ssb_deemp_FIR[MAX_RX_CHANS];

CIir m_deemp_Biquad[MAX_RX_CHANS];      // see: tools/biquad.MZT.m

#define N_NFM_DEEMP 2
#define N_NFM_DEEMP_TAPS 79

const float nfm_deemp_12000[N_NFM_DEEMP][N_NFM_DEEMP_TAPS] = {
    {
        // -LF  mkdeemph(12000,0)
        0.000119627, 0.00106125, 0.00220788, 0.0034464, 0.00472381, 0.00584572, 0.00690647, 0.00783264, 0.00855376, 0.00915763, 0.00977427, 0.0102304, 0.0103171, 0.00999654, 0.00925866, 0.0077544, 0.00571379, 0.00316622, 0.00041259, -0.00276795, -0.00635228, -0.010075, -0.013874, -0.0180137, -0.0220914, -0.0255059, -0.0282868, -0.0314145, -0.0334402, -0.0348514, -0.035364, -0.0350805, -0.0317931, -0.0242126, -0.013788, -0.00283687, 0.0134225, 0.0408602, 0.105308, 0.237135, 0.105308, 0.0408602, 0.0134225, -0.00283687, -0.013788, -0.0242126, -0.0317931, -0.0350805, -0.035364, -0.0348514, -0.0334402, -0.0314145, -0.0282868, -0.0255059, -0.0220914, -0.0180137, -0.013874, -0.010075, -0.00635228, -0.00276795, 0.00041259, 0.00316622, 0.00571379, 0.0077544, 0.00925866, 0.00999654, 0.0103171, 0.0102304, 0.00977427, 0.00915763, 0.00855376, 0.00783264, 0.00690647, 0.00584572, 0.00472381, 0.0034464, 0.00220788, 0.00106125, 0.000119627
    },
    {
        // +LF  mkdeemph(12000,1)
        -0.000490291, -0.00029695, 1.96983e-05, 0.000371536, 0.000730129, 0.000939212, 0.0011139, 0.00121606, 0.00121221, 0.00121713, 0.00138518, 0.00158832, 0.00166397, 0.00160611, 0.00143105, 0.000835956, 5.09773e-05, -0.000885942, -0.001691, -0.00257208, -0.00351751, -0.00429784, -0.00487966, -0.00554022, -0.00594321, -0.0055738, -0.00450885, -0.00371265, -0.00189548, 0.000430924, 0.00347129, 0.00707147, 0.0132259, 0.0230787, 0.0352339, 0.0474936, 0.0642636, 0.0909686, 0.15159, 0.274335, 0.15159, 0.0909686, 0.0642636, 0.0474936, 0.0352339, 0.0230787, 0.0132259, 0.00707147, 0.00347129, 0.000430924, -0.00189548, -0.00371265, -0.00450885, -0.0055738, -0.00594321, -0.00554022, -0.00487966, -0.00429784, -0.00351751, -0.00257208, -0.001691, -0.000885942, 5.09773e-05, 0.000835956, 0.00143105, 0.00160611, 0.00166397, 0.00158832, 0.00138518, 0.00121713, 0.00121221, 0.00121606, 0.0011139, 0.000939212, 0.000730129, 0.000371536, 1.96983e-05, -0.00029695, -0.000490291
    }
};

const float nfm_deemp_20250[N_NFM_DEEMP][N_NFM_DEEMP_TAPS] = {
    {
        // -LF  mkdeemph(20250,0)
        -0.00409825, -0.00291161, -0.00211214, -0.000776801, 0.000117355, 0.0016334, 0.00276088, 0.00448091, 0.00566994, 0.00741624, 0.00865479, 0.0105257, 0.0118775, 0.0138341, 0.0150973, 0.0167423, 0.0175618, 0.0190128, 0.0199183, 0.0214027, 0.0219192, 0.0228776, 0.0230178, 0.0237466, 0.0234898, 0.0237705, 0.0228505, 0.0219309, 0.0189098, 0.0158597, 0.0113709, 0.00783475, 0.00255904, -0.00197942, -0.00961354, -0.0181223, -0.0365493, -0.0557832, -0.0977046, -0.187589, -0.0977046, -0.0557832, -0.0365493, -0.0181223, -0.00961354, -0.00197942, 0.00255904, 0.00783475, 0.0113709, 0.0158597, 0.0189098, 0.0219309, 0.0228505, 0.0237705, 0.0234898, 0.0237466, 0.0230178, 0.0228776, 0.0219192, 0.0214027, 0.0199183, 0.0190128, 0.0175618, 0.0167423, 0.0150973, 0.0138341, 0.0118775, 0.0105257, 0.00865479, 0.00741624, 0.00566994, 0.00448091, 0.00276088, 0.0016334, 0.000117355, -0.000776801, -0.00211214, -0.00291161, -0.00409825
    },
    {
        // +LF  mkdeemph(20250,1)
        -8.79431e-05, 0.000238008, 0.000539234, 0.000847213, 0.00109915, 0.00141609, 0.00173348, 0.00208336, 0.00233416, 0.00258272, 0.00276817, 0.00300842, 0.00319506, 0.00341297, 0.00345621, 0.00335167, 0.00298661, 0.00267861, 0.00235807, 0.00205706, 0.00142368, 0.000705463, -0.000202459, -0.00106912, -0.00223991, -0.00340289, -0.00502377, -0.00706294, -0.0102699, -0.0139109, -0.0181675, -0.0220574, -0.0267919, -0.0313276, -0.0377929, -0.0453574, -0.0603587, -0.0764009, -0.110198, -0.183155, -0.110198, -0.0764009, -0.0603587, -0.0453574, -0.0377929, -0.0313276, -0.0267919, -0.0220574, -0.0181675, -0.0139109, -0.0102699, -0.00706294, -0.00502377, -0.00340289, -0.00223991, -0.00106912, -0.000202459, 0.000705463, 0.00142368, 0.00205706, 0.00235807, 0.00267861, 0.00298661, 0.00335167, 0.00345621, 0.00341297, 0.00319506, 0.00300842, 0.00276817, 0.00258272, 0.00233416, 0.00208336, 0.00173348, 0.00141609, 0.00109915, 0.000847213, 0.000539234, 0.000238008, -8.79431e-05
    }
};

const float am_ssb_deemp_12000[N_NFM_DEEMP][N_NFM_DEEMP_TAPS] = {
    {
        // 75 uS  mkdeemph(12000,2)
        5.68002e-05, 6.38298e-05, 0.000105638, -4.16972e-19, 0.000118056, 7.97321e-05, 7.93326e-05, -9.06813e-07, 2.28207e-05, 1.2226e-05, 2.6077e-05, -1.18441e-06, 0.000118509, 0.000136347, 0.000231389, -1.01413e-19, 0.000273381, 0.000190434, 0.000195903, -2.32144e-06, 6.07499e-05, 3.3961e-05, 7.58849e-05, -3.62725e-06, 0.00038397, 0.000470256, 0.00085573, -2.71685e-18, 0.00119519, 0.000921703, 0.00106658, -1.4509e-05, 0.000447566, 0.000305649, 0.000877229, -5.8036e-05, 0.00959924, 0.0230426, 0.144618, 0.715581, 0.144618, 0.0230426, 0.00959924, -5.8036e-05, 0.000877229, 0.000305649, 0.000447566, -1.4509e-05, 0.00106658, 0.000921703, 0.00119519, -2.71685e-18, 0.00085573, 0.000470256, 0.00038397, -3.62725e-06, 7.58849e-05, 3.3961e-05, 6.07499e-05, -2.32144e-06, 0.000195903, 0.000190434, 0.000273381, -1.01413e-19, 0.000231389, 0.000136347, 0.000118509, -1.18441e-06, 2.6077e-05, 1.2226e-05, 2.28207e-05, -9.06813e-07, 7.93326e-05, 7.97321e-05, 0.000118056, -4.16972e-19, 0.000105638, 6.38298e-05, 5.68002e-05
    },
    {
        // 50 uS  mkdeemph(12000,3)
        3.32352e-05, 9.2836e-06, 8.14013e-05, -3.7816e-19, 9.09701e-05, 1.15965e-05, 4.64195e-05, -2.46029e-05, 4.20079e-05, -3.2915e-05, 4.80019e-05, -3.21343e-05, 6.93426e-05, 1.98307e-05, 0.000178301, -3.50453e-19, 0.000210659, 2.76974e-05, 0.000114628, -6.29833e-05, 0.000111827, -9.14306e-05, 0.000139687, -9.84114e-05, 0.00022467, 6.83955e-05, 0.000659399, -1.9006e-18, 0.000920979, 0.000134055, 0.000624084, -0.000393646, 0.000823869, -0.000822876, 0.00161478, -0.00157458, 0.00561675, 0.00335138, 0.111438, 0.822065, 0.111438, 0.00335138, 0.00561675, -0.00157458, 0.00161478, -0.000822876, 0.000823869, -0.000393646, 0.000624084, 0.000134055, 0.000920979, -1.9006e-18, 0.000659399, 6.83955e-05, 0.00022467, -9.84114e-05, 0.000139687, -9.14306e-05, 0.000111827, -6.29833e-05, 0.000114628, 2.76974e-05, 0.000210659, -3.50453e-19, 0.000178301, 1.98307e-05, 6.93426e-05, -3.21343e-05, 4.80019e-05, -3.2915e-05, 4.20079e-05, -2.46029e-05, 4.64195e-05, 1.15965e-05, 9.09701e-05, -3.7816e-19, 8.14013e-05, 9.2836e-06, 3.32352e-05
    }
};

const float am_ssb_deemp_20250[N_NFM_DEEMP][N_NFM_DEEMP_TAPS] = {
    {
        // 75 uS  mkdeemph(20250,2)
        -0.000138825, -0.00013832, -0.000145305, -8.41301e-05, -7.75014e-05, -3.83089e-05, -1.31859e-05, 1.98759e-05, -6.19164e-05, -4.52602e-05, -2.26139e-05, 3.04478e-05, -8.16097e-05, -9.38793e-05, -0.000193842, -0.000276138, -0.000418412, -0.000413244, -0.000232704, -2.39381e-05, -0.000505114, -0.000633467, -0.00071802, -0.000496042, -0.000443712, -0.000275217, -0.000166635, 0.000187826, -0.000385997, -0.000459213, -0.000401824, 0.000443015, -0.000897255, -0.00166539, -0.00433441, -0.00901798, -0.023839, -0.0516351, -0.144354, -0.556974, -0.144354, -0.0516351, -0.023839, -0.00901798, -0.00433441, -0.00166539, -0.000897255, 0.000443015, -0.000401824, -0.000459213, -0.000385997, 0.000187826, -0.000166635, -0.000275217, -0.000443712, -0.000496042, -0.00071802, -0.000633467, -0.000505114, -2.39381e-05, -0.000232704, -0.000413244, -0.000418412, -0.000276138, -0.000193842, -9.38793e-05, -8.16097e-05, 3.04478e-05, -2.26139e-05, -4.52602e-05, -6.19164e-05, 1.98759e-05, -1.31859e-05, -3.83089e-05, -7.75014e-05, -8.41301e-05, -0.000145305, -0.00013832, -0.000138825
    },
    {
        // 50 uS  mkdeemph(20250,3)
        -0.000109826, -5.89333e-05, -6.48392e-05, -1.37353e-05, -3.95779e-05, 1.01952e-05, -2.84602e-05, 1.95224e-06, -2.3284e-05, 1.49187e-05, -3.5232e-05, 2.90319e-07, -2.86092e-05, -1.28894e-05, -8.03507e-05, -7.65039e-05, -0.000207567, -0.000264807, -0.000229579, -2.1886e-05, -0.000432772, -0.000310414, -0.000329181, -9.89118e-05, -0.000217807, 2.41593e-05, -0.000147441, -1.2877e-05, -0.00018731, 0.000124599, -0.000310341, -4.13262e-05, -0.000427044, -7.93066e-05, -0.00195429, -0.0021694, -0.0112681, -0.0291372, -0.132801, -0.662595, -0.132801, -0.0291372, -0.0112681, -0.0021694, -0.00195429, -7.93066e-05, -0.000427044, -4.13262e-05, -0.000310341, 0.000124599, -0.00018731, -1.2877e-05, -0.000147441, 2.41593e-05, -0.000217807, -9.89118e-05, -0.000329181, -0.000310414, -0.000432772, -2.1886e-05, -0.000229579, -0.000264807, -0.000207567, -7.65039e-05, -8.03507e-05, -1.28894e-05, -2.86092e-05, 2.90319e-07, -3.5232e-05, 1.49187e-05, -2.3284e-05, 1.95224e-06, -2.84602e-05, 1.01952e-05, -3.95779e-05, -1.37353e-05, -6.48392e-05, -5.89333e-05, -0.000109826
    }
};
