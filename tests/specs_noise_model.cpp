/*
* Copyright (c) 2019, Intel Corporation
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the Intel Corporation nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "Teisko/NoiseModel.hpp"
#include "catch.hpp"

SCENARIO("Calculate noise models in liblab library", "[NoiseModel]")
{
    GIVEN("a known set of mean and noisy standard deviation values")
    {
        std::vector<double> means = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
            11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
            21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
            31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
            41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
            51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
            61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
            71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
            81, 82, 83, 84, 85, 86, 87, 88, 89, 90,
            91, 92, 93, 94, 95, 96, 97, 98, 99, 100 };

        std::vector<double> stds = { 9.9726, 15.123, 19.774, 24.937, 29.764, 34.648, 39.582, 44.479, 49.083,
            54.132, 58.669, 63.453, 68.243, 73.114, 77.643, 82.473, 87.151, 91.775, 96.445, 100.88,
            105.69, 110.17, 114.78, 119.14, 123.76, 128.33, 132.81, 137.15, 141.4, 145.89, 150.41,
            154.84, 159.16, 163.38, 167.97, 172.05, 176.4, 180.52, 184.68, 189.07, 193.29, 197.54,
            201.46, 205.48, 209.91, 213.87, 217.8, 221.9, 225.91, 230.12, 234.05, 237.98, 241.93,
            245.76, 249.85, 253.67, 257.48, 261.34, 265.18, 268.91, 272.86, 276.7, 280.56, 284.09, 287.67,
            291.46, 295.01, 298.7, 302.32, 305.92, 309.63, 313.05, 316.78, 320.32, 323.85, 327.24, 330.77,
            334.34, 337.6, 340.98, 344.57, 347.51, 351.06, 354.68, 357.7, 360.98, 364.19, 367.64, 370.58,
            373.92, 377.25, 380.36, 383.57, 386.65, 389.82, 392.71, 395.93, 398.8, 402.07, 404.97 };

        THEN("noise model calculation should return successfully")
        {
            // y = const_a + coeff_y * x + coeff_y_pow2 * x^2
            double const_a = 5.00;
            double coeff_y = 5.00;
            double coeff_y_pow2 = -0.010;

            Teisko::noise_model noise_model;

            auto res = Teisko::calculate_y_noise_model(means, stds, noise_model);

            CHECK(Teisko::NoiseModel::SUCCESS == res);

            AND_THEN("noise model result should be close to reference values")
            {
                CHECK(const_a == Approx(noise_model.c3).epsilon(1e-2));
                CHECK(coeff_y == Approx(noise_model.c2).epsilon(1e-2));
                CHECK(coeff_y_pow2 == Approx(noise_model.c1).epsilon(1e-4));
            }
        }
    }

    GIVEN("a known set of mean and noisy standard deviation values for non negative least squares fit")
    {
        std::vector<double> means = { 0.07424, 0.2624, 0.1608, 0.1138, 0.2108, 0.3077,
            0.2232, 0.1174, 0.1842, 0.069, 0.3501, 0.2931,
            0.05809, 0.1911, 0.1167, 0.5069, 0.1898, 0.1677,
            0.5396, 0.4564, 0.294, 0.1483, 0.06624, 0.02593 };

        std::vector<double> stds = { 1.5866e-03, 3.4200e-03, 2.4936e-03, 2.0367e-03, 2.7681e-03, 3.7738e-03,
            2.9093e-03, 2.0136e-03, 2.5369e-03, 1.5389e-03, 4.0076e-03, 3.9276e-03,
            1.4309e-03, 2.6661e-03, 1.9527e-03, 4.9931e-03, 2.6220e-03, 2.4618e-03,
            5.8759e-03, 4.8861e-03, 3.7551e-03, 2.3830e-03, 1.6098e-03, 8.8106e-04 };

        // TODO: This does compile on Linux but aborts at runtime.
        // Originally LIBLAB models were supported on Windows only,
        // but naturally it would be great to fix this also.
        #ifdef _MSC_VER
            THEN("noise model calculation should return successfully")
            {
                // y = const_a + coeff_y * x + coeff_y_pow2 * x^2
                double const_a = 7.762667236e-07;
                double coeff_y = 2.2305126674931382e-05;
                double coeff_y_pow2 = 6.4639203305957399e-05;

                Teisko::noise_model noise_model;

                auto res = Teisko::calculate_noise_model_nonneg(means, stds, noise_model);

                CHECK(Teisko::NoiseModel::SUCCESS == res);

                AND_THEN("noise model result should be close to reference values")
                {
                    CHECK(const_a == Approx(noise_model.c3).epsilon(1e-9));
                    CHECK(coeff_y == Approx(noise_model.c2).epsilon(1e-7));
                    CHECK(coeff_y_pow2 == Approx(noise_model.c1).epsilon(1e-7));
                }
            }
        #endif
    }

    GIVEN("a noisy patch and it's coordinates from an image")
    {
        std::vector<double> values = { 5.95834287e-01, 6.04959182e-01, 5.99771115e-01, 6.13443198e-01, 6.22018769e-01, 6.20248722e-01, 6.07858396e-01, 6.05477989e-01, 5.84176394e-01, 6.00473030e-01, 6.05142290e-01, 6.09048600e-01, 6.09567407e-01, 6.22934310e-01, 6.20340276e-01,
            6.08834974e-01, 6.06057832e-01, 6.12130922e-01, 6.12985428e-01, 6.27389944e-01, 6.07034409e-01, 5.98916609e-01, 6.10818647e-01, 6.07492180e-01, 5.90157931e-01, 6.08499275e-01, 6.24551766e-01, 6.04043641e-01, 6.10116732e-01, 6.12283513e-01,
            6.12191958e-01, 6.14938582e-01, 6.31662470e-01, 6.26901656e-01, 6.04318303e-01, 6.09903105e-01, 6.26413367e-01, 6.12741283e-01, 6.10727092e-01, 6.07614252e-01, 6.00991836e-01, 6.00778210e-01, 6.15213245e-01, 6.12039368e-01, 6.05661097e-01,
            6.07644770e-01, 6.11795224e-01, 6.26504921e-01, 6.35049973e-01, 6.18936446e-01, 6.31754025e-01, 6.33279927e-01, 6.19638361e-01, 6.10849165e-01, 6.13626307e-01, 6.05569543e-01, 6.02334630e-01, 6.22964828e-01, 6.16952773e-01, 6.09689479e-01,
            6.07003891e-01, 6.26169223e-01, 6.30899519e-01, 6.17959869e-01, 6.24643320e-01, 6.20523384e-01, 6.19974060e-01, 6.24307622e-01, 6.22171359e-01, 5.98702983e-01, 5.99496452e-01, 6.26291295e-01, 6.30563821e-01, 6.18753338e-01, 6.12344549e-01,
            6.23422599e-01, 6.19211109e-01, 6.13992523e-01, 5.93942168e-01, 6.02670329e-01, 6.09781033e-01, 6.10482948e-01, 6.18753338e-01, 6.16098268e-01, 6.00778210e-01, 5.97543297e-01, 6.16861219e-01, 6.22842756e-01, 6.14511330e-01, 6.19394217e-01,
            5.95498589e-01, 6.02517739e-01, 6.10605020e-01, 6.06881819e-01, 6.04470893e-01, 5.97085527e-01, 5.84512093e-01, 6.02395666e-01, 6.10635538e-01, 5.99313344e-01, 5.91592279e-01, 6.10299840e-01, 6.21408408e-01, 6.15335317e-01, 6.23422599e-01,
            6.00656138e-01, 5.99923705e-01, 6.03585870e-01, 5.95925841e-01, 6.09872587e-01, 6.15152209e-01, 6.04989700e-01, 6.02670329e-01, 6.00259403e-01, 6.09597925e-01, 6.05844205e-01, 6.04684520e-01, 6.05966278e-01, 6.26474403e-01, 6.22598611e-01,
            6.10818647e-01, 6.03738460e-01, 6.17471580e-01, 5.89303426e-01, 6.04837110e-01, 6.13992523e-01, 6.23666743e-01, 6.24155032e-01, 6.07553216e-01, 6.09048600e-01, 6.10971237e-01, 6.04989700e-01, 6.10543984e-01, 6.24307622e-01, 6.16678111e-01,
            6.05050736e-01, 6.02975509e-01, 5.99588006e-01, 5.74685283e-01, 5.96078431e-01, 5.99496452e-01, 6.10849165e-01, 6.13351644e-01, 6.16037232e-01, 6.11642634e-01, 6.15335317e-01, 6.07949950e-01, 6.18417639e-01, 6.24826429e-01, 6.20920119e-01,
            6.16556039e-01, 6.18692302e-01, 6.19150072e-01, 5.99099718e-01, 5.98947127e-01, 6.05111772e-01, 6.22171359e-01, 6.09292744e-01, 6.03646906e-01, 6.18417639e-01, 6.23056382e-01, 6.17288472e-01, 6.15915160e-01, 6.15609979e-01, 6.18814374e-01,
            6.14236667e-01, 6.26901656e-01, 6.15854124e-01, 6.05935760e-01, 6.02334630e-01, 6.19455253e-01, 6.20614939e-01, 6.21988251e-01, 6.24643320e-01, 6.16098268e-01, 6.05813687e-01, 6.14389258e-01, 6.15487907e-01, 6.17257954e-01, 6.18387121e-01,
            6.10910201e-01, 6.11490043e-01, 6.09140154e-01, 6.17074846e-01, 6.15457389e-01, 6.13656825e-01, 6.11764706e-01, 6.13656825e-01, 6.26504921e-01, 6.14785992e-01, 5.99679561e-01, 6.03250172e-01, 6.15213245e-01, 6.20035096e-01, 6.13870451e-01,
            6.17593652e-01, 6.05264363e-01, 6.00869764e-01, 6.12527657e-01, 6.22201877e-01, 6.10727092e-01, 6.06088350e-01, 6.02304112e-01, 5.98245212e-01, 5.97604334e-01, 5.97970550e-01, 6.03646906e-01, 6.14877546e-01, 6.12344549e-01, 6.07736324e-01,
            6.19302663e-01, 6.01815824e-01, 5.88570993e-01, 5.96475166e-01, 6.30441749e-01, 6.22201877e-01, 6.08407721e-01, 6.03433280e-01, 6.06088350e-01, 6.06057832e-01, 5.97665370e-01, 6.00808728e-01, 6.08926528e-01, 6.10177768e-01, 6.00778210e-01 };

        std::vector<double> xcoords = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
            5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
            6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
            7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
            8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
            9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
            10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
            11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
            12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
            13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
            14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
            15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 };

        std::vector<double> ycoords = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

        std::vector<double> surface_ref = { 6.11998653e-01, 6.11204327e-01, 6.10550142e-01, 6.10036098e-01, 6.09662195e-01, 6.09428434e-01, 6.09334813e-01, 6.09381333e-01, 6.09567994e-01, 6.09894797e-01, 6.10361740e-01, 6.10968824e-01, 6.11716049e-01, 6.12603416e-01, 6.13630923e-01,
            6.12383367e-01, 6.11607147e-01, 6.10971068e-01, 6.10475130e-01, 6.10119333e-01, 6.09903676e-01, 6.09828161e-01, 6.09892787e-01, 6.10097554e-01, 6.10442462e-01, 6.10927511e-01, 6.11552701e-01, 6.12318032e-01, 6.13223504e-01, 6.14269117e-01,
            6.12664494e-01, 6.11906380e-01, 6.11288406e-01, 6.10810574e-01, 6.10472883e-01, 6.10275332e-01, 6.10217923e-01, 6.10300654e-01, 6.10523527e-01, 6.10886541e-01, 6.11389695e-01, 6.12032991e-01, 6.12816428e-01, 6.13740005e-01, 6.14803724e-01,
            6.12842035e-01, 6.12102026e-01, 6.11502158e-01, 6.11042431e-01, 6.10722846e-01, 6.10543401e-01, 6.10504097e-01, 6.10604934e-01, 6.10845913e-01, 6.11227032e-01, 6.11748292e-01, 6.12409694e-01, 6.13211236e-01, 6.14152919e-01, 6.15234744e-01,
            6.12915988e-01, 6.12194085e-01, 6.11612323e-01, 6.11170701e-01, 6.10869221e-01, 6.10707882e-01, 6.10686684e-01, 6.10805627e-01, 6.11064711e-01, 6.11463936e-01, 6.12003302e-01, 6.12682809e-01, 6.13502457e-01, 6.14462246e-01, 6.15562177e-01,
            6.12886354e-01, 6.12182556e-01, 6.11618900e-01, 6.11195385e-01, 6.10912010e-01, 6.10768777e-01, 6.10765684e-01, 6.10902733e-01, 6.11179923e-01, 6.11597253e-01, 6.12154725e-01, 6.12852338e-01, 6.13690092e-01, 6.14667986e-01, 6.15786022e-01,
            6.12753133e-01, 6.12067441e-01, 6.11521890e-01, 6.11116481e-01, 6.10851212e-01, 6.10726084e-01, 6.10741098e-01, 6.10896252e-01, 6.11191547e-01, 6.11626984e-01, 6.12202561e-01, 6.12918279e-01, 6.13774139e-01, 6.14770139e-01, 6.15906281e-01,
            6.12516325e-01, 6.11848739e-01, 6.11321294e-01, 6.10933990e-01, 6.10686826e-01, 6.10579804e-01, 6.10612923e-01, 6.10786184e-01, 6.11099585e-01, 6.11553127e-01, 6.12146810e-01, 6.12880634e-01, 6.13754599e-01, 6.14768705e-01, 6.15922952e-01,
            6.12175929e-01, 6.11526449e-01, 6.11017110e-01, 6.10647911e-01, 6.10418854e-01, 6.10329938e-01, 6.10381162e-01, 6.10572528e-01, 6.10904035e-01, 6.11375683e-01, 6.11987471e-01, 6.12739401e-01, 6.13631472e-01, 6.14663684e-01, 6.15836037e-01,
            6.11731947e-01, 6.11100572e-01, 6.10609339e-01, 6.10258246e-01, 6.10047294e-01, 6.09976484e-01, 6.10045814e-01, 6.10255286e-01, 6.10604898e-01, 6.11094651e-01, 6.11724546e-01, 6.12494581e-01, 6.13404758e-01, 6.14455075e-01, 6.15645534e-01,
            6.11184378e-01, 6.10571109e-01, 6.10097981e-01, 6.09764994e-01, 6.09572148e-01, 6.09519443e-01, 6.09606879e-01, 6.09834456e-01, 6.10202174e-01, 6.10710033e-01, 6.11358033e-01, 6.12146174e-01, 6.13074457e-01, 6.14142880e-01, 6.15351444e-01,
            6.10533221e-01, 6.09938058e-01, 6.09483036e-01, 6.09168154e-01, 6.08993414e-01, 6.08958815e-01, 6.09064356e-01, 6.09310039e-01, 6.09695863e-01, 6.10221828e-01, 6.10887934e-01, 6.11694180e-01, 6.12640568e-01, 6.13727097e-01, 6.14953767e-01,
            6.09778478e-01, 6.09201420e-01, 6.08764503e-01, 6.08467728e-01, 6.08311093e-01, 6.08294600e-01, 6.08418247e-01, 6.08682035e-01, 6.09085965e-01, 6.09630035e-01, 6.10314247e-01, 6.11138599e-01, 6.12103093e-01, 6.13207728e-01, 6.14452503e-01,
            6.08920147e-01, 6.08361195e-01, 6.07942384e-01, 6.07663714e-01, 6.07525185e-01, 6.07526797e-01, 6.07668550e-01, 6.07950445e-01, 6.08372480e-01, 6.08934656e-01, 6.09636973e-01, 6.10479431e-01, 6.11462031e-01, 6.12584771e-01, 6.13847652e-01,
            6.07958229e-01, 6.07417383e-01, 6.07016678e-01, 6.06756113e-01, 6.06635690e-01, 6.06655408e-01, 6.06815267e-01, 6.07115267e-01, 6.07555407e-01, 6.08135689e-01, 6.08856112e-01, 6.09716676e-01, 6.10717381e-01, 6.11858227e-01, 6.13139214e-01 };

        THEN("surface fit should return succesfully")
        {
            auto surface = std::vector<double>(values.size());
            auto res = Teisko::fit_surface(values, xcoords, ycoords, surface);

            CHECK(Teisko::NoiseModel::SUCCESS == res);

            AND_THEN("surface fit should be close to reference")
            {
                for (size_t idx = 0; idx < surface.size(); ++idx)
                {
                    CHECK(surface_ref[idx] == Approx(surface[idx]).epsilon(1e-6));
                }
            }
        }
    }

    GIVEN("a known set of means, variances, weights, and gains for non-negative least squares solver for SVE")
    {
        std::vector<double> means = { 1.94320988e+01, 1.96296296e+01, 7.28395062e+01, 8.28518519e+01, 2.43703704e+01, 1.07691358e+02, 1.05543210e+02, 1.26444444e+02, 2.77777778e+01, 9.15308642e+01,
            9.56666667e+01, 3.91111111e+01, 3.48888889e+01, 5.39876543e+01, 3.77777778e+01, 3.69506173e+01, 6.09753086e+01, 4.23827160e+01, 1.98765432e+01, 5.59753086e+01,
            7.69259259e+01, 7.58148148e+01, 1.05333333e+02, 2.49962963e+02, 1.75345679e+02, 1.14049383e+02, 1.16037037e+02, 6.02469136e+01, 2.79876543e+01, 1.08518519e+01,
            4.05432099e+01, 4.05555556e+01, 1.47135802e+02, 9.39506173e+01, 6.11234568e+01, 1.15197531e+02, 2.10320988e+02, 2.05604938e+02, 9.62592593e+01, 7.11851852e+01,
            7.02098765e+01, 7.19135802e+01, 3.49259259e+01, 2.03419753e+02, 1.52950617e+02, 3.59506173e+01, 1.15098765e+02, 1.19716049e+02, 4.54074074e+01, 2.55320988e+02,
            8.59382716e+01, 1.24777778e+02, 1.22814815e+02, 3.88888889e+02, 2.71641975e+02, 1.82901235e+02, 9.77530864e+01, 4.54074074e+01, 4.47777778e+01, 1.70493827e+01,
            3.98148148e+01, 1.41456790e+02, 9.77654321e+01, 9.94444444e+01, 6.38888889e+01, 1.21123457e+02, 2.06209877e+02, 9.29753086e+01, 9.76543210e+01, 7.14938272e+01,
            7.00740741e+01, 3.48148148e+01, 2.05061728e+02, 2.00012346e+02, 1.56740741e+02, 3.71234568e+01, 1.15851852e+02, 4.46419753e+01, 4.67160494e+01, 2.54382716e+02,
            8.58641975e+01, 1.24432099e+02, 3.83086420e+02, 2.68543210e+02, 2.79024691e+02, 1.79432099e+02, 9.47901235e+01, 4.36543210e+01, 1.67654321e+01, 1.79382716e+01,
            2.50864198e+01, 8.79012346e+01, 2.98888889e+01, 2.35555556e+01, 2.35185185e+01, 4.89259259e+01, 5.12839506e+01, 9.08148148e+01, 2.07654321e+01, 2.04814815e+01,
            8.56790123e+01, 2.33950617e+01, 7.55555556e+01, 1.15493827e+02, 1.08703704e+02, 9.33333333e+00, 2.73827160e+01, 7.31604938e+01, 1.50901235e+02, 1.49777778e+02,
            8.68148148e+01, 2.16419753e+01, 1.59728395e+02, 1.13518519e+02, 7.30000000e+01, 7.52839506e+01, 4.04567901e+01, 1.80000000e+01, 6.64197531e+00, 2.33333333e+01,
            2.22592593e+01, 7.58271605e+01, 8.49753086e+01, 2.43950617e+01, 1.05185185e+02, 1.28691358e+02, 1.20481481e+02, 2.90617284e+01, 9.59259259e+01, 4.22839506e+01,
            3.75308642e+01, 3.37160494e+01, 5.24814815e+01, 3.56419753e+01, 6.09876543e+01, 4.31604938e+01, 1.99753086e+01, 2.02839506e+01, 5.58765432e+01, 7.35061728e+01,
            1.00592593e+02, 2.37975309e+02, 2.47197531e+02, 1.81925926e+02, 1.17444444e+02, 5.80617284e+01, 2.91358025e+01, 2.59506173e+01, 1.02716049e+01, 4.23703704e+01,
            1.46814815e+02, 1.02148148e+02, 1.04444444e+02, 6.35555556e+01, 1.10209877e+02, 2.01370370e+02, 9.63827160e+01, 7.40987654e+01, 7.66543210e+01, 7.20864198e+01,
            3.42716049e+01, 1.97716049e+02, 1.52975309e+02, 1.46765432e+02, 3.91975309e+01, 1.21493827e+02, 4.69506173e+01, 2.57703704e+02, 8.36049383e+01, 7.99135802e+01,
            1.19555556e+02, 3.91320988e+02, 2.72148148e+02, 1.73333333e+02, 1.74938272e+02, 9.10617284e+01, 4.50370370e+01, 1.66913580e+01, 4.17407407e+01, 4.09753086e+01,
            1.48320988e+02, 9.87037037e+01, 6.49876543e+01, 1.20888889e+02, 1.16765432e+02, 2.05148148e+02, 1.01370370e+02, 7.25555556e+01, 7.19259259e+01, 3.42716049e+01,
            3.40246914e+01, 1.95271605e+02, 1.54012346e+02, 3.63333333e+01, 1.17086420e+02, 1.20283951e+02, 4.57654321e+01, 2.51160494e+02, 8.15061728e+01, 1.22111111e+02,
            1.15123457e+02, 4.03382716e+02, 2.78407407e+02, 1.80160494e+02, 9.18518519e+01, 4.32592593e+01, 4.21481481e+01, 1.57901235e+01, 2.77160494e+01, 8.83827160e+01,
            2.92592593e+01, 2.89135802e+01, 2.49012346e+01, 4.80987654e+01, 4.79382716e+01, 9.27777778e+01, 1.01197531e+02, 2.28395062e+01, 8.80493827e+01, 2.40000000e+01,
            7.13950617e+01, 1.09123457e+02, 1.01654321e+02, 1.07037037e+01, 2.95679012e+01, 7.25679012e+01, 1.49061728e+02, 1.47691358e+02, 8.66049383e+01, 2.06172840e+01,
            1.54592593e+02, 1.12703704e+02, 1.20370370e+02, 7.57283951e+01, 4.12469136e+01, 1.70123457e+01, 6.93827160e+00, 7.18518519e+00, 2.43333333e+01, 7.50493827e+01,
            7.55061728e+01, 1.87530864e+01, 9.50370370e+01, 8.84567901e+01, 1.02555556e+02, 2.27160494e+01, 8.13333333e+01, 3.30246914e+01, 4.49753086e+01, 3.22962963e+01,
            4.90987654e+01, 3.03950617e+01, 4.86543210e+01, 5.46296296e+01, 4.36049383e+01, 1.82098765e+01, 5.41358025e+01, 7.50493827e+01, 8.95802469e+01, 8.00370370e+01,
            2.26012346e+02, 1.47111111e+02, 1.00864198e+02, 4.60246914e+01, 6.01481481e+01, 3.20123457e+01, 4.90123457e+00, 4.36913580e+01, 1.18691358e+02, 1.36185185e+02,
            8.59012346e+01, 4.73086420e+01, 1.02407407e+02, 1.87197531e+02, 1.64728395e+02, 8.64197531e+01, 7.14444444e+01, 6.32962963e+01, 2.98641975e+01, 1.77000000e+02,
            1.75814815e+02, 1.22481481e+02, 3.11234568e+01, 1.12703704e+02, 3.52592593e+01, 3.98395062e+01, 2.24950617e+02, 6.28888889e+01, 9.95308642e+01, 3.42296296e+02,
            2.46037037e+02, 2.57518519e+02, 1.65271605e+02, 7.68888889e+01, 3.42469136e+01, 1.17160494e+01, 1.63333333e+01, 3.42345679e+01, 1.29234568e+02, 8.58024691e+01,
            5.51111111e+01, 1.10271605e+02, 1.03666667e+02, 1.69629630e+02, 8.26790123e+01, 6.08148148e+01, 6.76172840e+01, 7.11851852e+01, 2.90740741e+01, 1.63246914e+02,
            1.35209877e+02, 2.92345679e+01, 3.38765432e+01, 1.13876543e+02, 3.82222222e+01, 2.22123457e+02, 7.53456790e+01, 1.00098765e+02, 9.81604938e+01, 3.60716049e+02,
            2.43802469e+02, 1.64049383e+02, 8.17037037e+01, 8.41728395e+01, 3.39259259e+01, 1.65555556e+01, 2.65555556e+01, 7.94567901e+01, 8.30123457e+01, 3.07037037e+01,
            2.19506173e+01, 4.36419753e+01, 4.96296296e+01, 7.77654321e+01, 8.66666667e+01, 1.34320988e+01, 8.69135802e+01, 2.32716049e+01, 7.53580247e+01, 5.33456790e+01,
            9.97283951e+01, 5.48148148e+00, 3.00246914e+01, 6.07160494e+01, 1.41530864e+02, 1.41333333e+02, 7.97407407e+01, 1.56790123e+01, 1.48567901e+02, 1.06913580e+02,
            1.03123457e+02, 6.18148148e+01, 3.44197531e+01, 1.21975309e+01, 2.90123457e+00, 1.28518519e+01 };

        std::vector<double> vars = { 3.39845679e+00, 3.61111111e+00, 7.11141975e+00, 1.00277778e+01, 4.83611111e+00, 1.12410494e+01, 1.11262346e+01, 2.08750000e+01, 5.15000000e+00, 1.20771605e+01,
            9.80000000e+00, 5.22500000e+00, 4.90000000e+00, 5.18734568e+00, 5.37500000e+00, 4.14753086e+00, 7.49938272e+00, 6.13919753e+00, 3.33456790e+00, 9.07438272e+00,
            7.64444444e+00, 7.62777778e+00, 9.05000000e+00, 2.40611111e+01, 2.14790123e+01, 1.31725309e+01, 1.67361111e+01, 7.31327160e+00, 3.31234568e+00, 3.20277778e+00,
            6.37623457e+00, 7.75000000e+00, 1.86688272e+01, 1.27225309e+01, 9.20956790e+00, 1.28854938e+01, 2.26956790e+01, 2.26919753e+01, 1.34694444e+01, 9.07777778e+00,
            8.54290123e+00, 8.87993827e+00, 4.89444444e+00, 2.34966049e+01, 1.85725309e+01, 5.89753086e+00, 1.40901235e+01, 1.44058642e+01, 7.61944444e+00, 3.28956790e+01,
            1.43836420e+01, 1.35750000e+01, 1.61027778e+01, 5.55000000e+01, 5.07577160e+01, 2.24151235e+01, 1.53382716e+01, 6.84444444e+00, 7.42500000e+00, 4.17253086e+00,
            5.52777778e+00, 1.52512346e+01, 1.49567901e+01, 1.35000000e+01, 1.00750000e+01, 1.50095679e+01, 2.53429012e+01, 1.29993827e+01, 1.93790123e+01, 8.72808642e+00,
            1.23194444e+01, 6.27777778e+00, 2.48336420e+01, 2.90123457e+01, 1.97944444e+01, 5.88456790e+00, 1.62527778e+01, 6.60771605e+00, 7.78086420e+00, 4.47641975e+01,
            1.09188272e+01, 1.44234568e+01, 4.85549383e+01, 2.97512346e+01, 4.06243827e+01, 2.23234568e+01, 1.17929012e+01, 5.15401235e+00, 3.00679012e+00, 3.08364198e+00,
            2.75493827e+00, 1.34651235e+01, 5.02500000e+00, 5.40000000e+00, 4.57777778e+00, 8.81944444e+00, 8.70586420e+00, 1.30527778e+01, 3.43179012e+00, 3.75277778e+00,
            1.30456790e+01, 5.86697531e+00, 7.57500000e+00, 1.34780864e+01, 1.40861111e+01, 2.52500000e+00, 5.18919753e+00, 1.00864198e+01, 2.14651235e+01, 1.77000000e+01,
            1.07027778e+01, 3.75771605e+00, 1.95003086e+01, 1.44527778e+01, 1.12750000e+01, 6.33086420e+00, 7.02623457e+00, 3.32500000e+00, 3.48271605e+00, 5.13000000e+01,
            5.50694444e+01, 1.10669753e+02, 1.68524383e+02, 6.41669753e+01, 1.90902778e+02, 2.53866049e+02, 2.53002778e+02, 8.02336420e+01, 1.37769444e+02, 7.39558642e+01,
            6.55021605e+01, 7.98308642e+01, 5.75277778e+01, 1.05257716e+02, 1.35362346e+02, 8.41114198e+01, 5.48243827e+01, 5.36058642e+01, 5.65595679e+01, 1.43503086e+02,
            1.96194444e+02, 5.25399383e+02, 3.76385494e+02, 2.52694444e+02, 1.93425000e+02, 1.05483642e+02, 6.28438272e+01, 5.45975309e+01, 2.36753086e+01, 7.46861111e+01,
            2.80302778e+02, 2.29052778e+02, 2.34875000e+02, 1.46050000e+02, 2.43142901e+02, 2.28436111e+02, 1.82039198e+02, 1.17690123e+02, 1.08004012e+02, 1.30029938e+02,
            6.99003086e+01, 3.11055864e+02, 2.65149383e+02, 3.57206790e+02, 5.69854938e+01, 2.27178086e+02, 8.96975309e+01, 4.31386111e+02, 1.50066975e+02, 1.32579938e+02,
            2.26725000e+02, 7.07420679e+02, 4.05302778e+02, 3.52600000e+02, 2.69808642e+02, 2.01508642e+02, 7.51611111e+01, 4.91160494e+01, 6.98194444e+01, 9.01243827e+01,
            3.13120679e+02, 1.69811111e+02, 1.32187346e+02, 2.97225000e+02, 2.60106790e+02, 3.70127778e+02, 1.79486111e+02, 1.52300000e+02, 1.06469444e+02, 7.17753086e+01,
            5.47743827e+01, 3.33225309e+02, 1.94287346e+02, 5.32000000e+01, 1.94929938e+02, 2.08830864e+02, 1.06706790e+02, 3.40961420e+02, 1.33578086e+02, 2.09300000e+02,
            1.75959568e+02, 7.61439198e+02, 4.80994444e+02, 3.13236420e+02, 1.75827778e+02, 8.26944444e+01, 8.93527778e+01, 3.99429012e+01, 6.26808642e+01, 1.67289198e+02,
            7.32444444e+01, 7.31299383e+01, 5.30651235e+01, 1.02065123e+02, 6.36836420e+01, 1.49175000e+02, 2.09285494e+02, 5.66364198e+01, 1.21322531e+02, 5.38250000e+01,
            1.19891975e+02, 1.45634568e+02, 1.47579012e+02, 2.98611111e+01, 6.91234568e+01, 2.13348457e+02, 2.93283642e+02, 2.41066049e+02, 1.22916975e+02, 4.11391975e+01,
            2.67519444e+02, 1.92261111e+02, 2.07436111e+02, 1.70175309e+02, 8.88382716e+01, 4.47873457e+01, 2.55586420e+01, 3.35777778e+01, 9.78925000e+02, 1.90719753e+03,
            2.08445309e+03, 7.72938272e+02, 2.05111111e+03, 2.16022623e+03, 2.60725000e+03, 8.80205864e+02, 2.80370000e+03, 1.55292438e+03, 1.25517438e+03, 7.55486111e+02,
            1.39899012e+03, 1.23884198e+03, 1.67127901e+03, 1.25123611e+03, 1.33654198e+03, 9.79867901e+02, 1.10551883e+03, 1.24102253e+03, 1.84144660e+03, 2.07128611e+03,
            4.22306235e+03, 3.18610000e+03, 1.88781883e+03, 1.21477438e+03, 1.63270278e+03, 1.08748735e+03, 6.30565123e+02, 9.51791049e+02, 2.54964105e+03, 2.51840278e+03,
            2.15666512e+03, 1.87964105e+03, 2.55401944e+03, 3.12126049e+03, 2.75007531e+03, 1.69669660e+03, 1.86390000e+03, 1.79843611e+03, 1.31426883e+03, 4.04580000e+03,
            3.11105278e+03, 1.93395278e+03, 1.22090957e+03, 2.55618611e+03, 1.05801944e+03, 1.54846142e+03, 2.99217253e+03, 1.60927500e+03, 1.84947716e+03, 4.31783611e+03,
            4.69938611e+03, 3.60052778e+03, 3.48337531e+03, 1.58875000e+03, 1.45801327e+03, 8.39480864e+02, 9.47150000e+02, 1.32540679e+03, 2.65523179e+03, 1.88776049e+03,
            1.20270000e+03, 1.84460031e+03, 2.46760000e+03, 2.73021111e+03, 1.71259568e+03, 1.84122778e+03, 1.40391420e+03, 1.73927778e+03, 1.09351944e+03, 2.44426327e+03,
            1.75951790e+03, 1.34153179e+03, 1.11810957e+03, 2.46838457e+03, 1.09780000e+03, 4.92685957e+03, 1.84617901e+03, 1.63576512e+03, 1.96208642e+03, 7.37218086e+03,
            5.14591049e+03, 3.75029753e+03, 2.42773611e+03, 1.94441975e+03, 1.44874444e+03, 8.88625000e+02, 9.42950000e+02, 1.60925123e+03, 1.78603735e+03, 1.04993611e+03,
            8.95647531e+02, 1.98020772e+03, 1.27258611e+03, 1.38433179e+03, 2.08592500e+03, 9.01273457e+02, 1.79517994e+03, 9.51725309e+02, 1.69275772e+03, 9.87429012e+02,
            1.99112531e+03, 9.55777778e+02, 8.68599383e+02, 1.82058086e+03, 2.85115216e+03, 2.70870000e+03, 1.69869444e+03, 8.06345679e+02, 2.64274846e+03, 2.13492994e+03,
            2.46245957e+03, 1.82260278e+03, 1.35912160e+03, 9.73185494e+02, 9.28890123e+02, 8.08077778e+02 };

        std::vector<double> weights = { 1.44485770e+00, 1.44485770e+00, 6.84801119e-01, 6.45295505e-01, 1.40355845e+00, 4.53317128e-01, 4.53317128e-01, 3.57877948e-01, 1.45872486e+00, 5.73690640e-01,
            5.73690640e-01, 1.13490262e+00, 1.01782916e+00, 7.73194779e-01, 1.01892631e+00, 1.01892631e+00, 7.87841813e-01, 8.87596342e-01, 1.87463110e+00, 8.19711522e-01,
            6.01417753e-01, 6.01417753e-01, 4.42305944e-01, 2.03022459e-01, 3.37663139e-01, 4.47253192e-01, 4.47253192e-01, 7.33603141e-01, 1.41201567e+00, 1.96607803e+00,
            1.06377550e+00, 1.06377550e+00, 3.01173526e-01, 4.89646563e-01, 7.50902428e-01, 3.93089534e-01, 2.35756636e-01, 2.35756636e-01, 4.95135857e-01, 5.73372532e-01,
            5.78803853e-01, 5.78803853e-01, 1.08147322e+00, 2.24040066e-01, 2.83548166e-01, 8.09819189e-01, 3.81611793e-01, 3.81611793e-01, 7.97868328e-01, 1.81444875e-01,
            5.09092396e-01, 3.95990826e-01, 3.95990826e-01, 1.12130943e-01, 1.56314817e-01, 2.57164506e-01, 4.73691853e-01, 9.07828660e-01, 9.07828660e-01, 1.40661534e+00,
            1.12484262e+00, 3.15146065e-01, 4.86578538e-01, 4.86578538e-01, 6.52973354e-01, 3.77444318e-01, 2.20865243e-01, 5.02986344e-01, 4.62497354e-01, 6.63393565e-01,
            6.63351769e-01, 1.16262990e+00, 2.22377562e-01, 2.22377562e-01, 2.85009082e-01, 6.58455927e-01, 3.89911309e-01, 9.66146780e-01, 9.66146780e-01, 1.80322626e-01,
            5.11968356e-01, 3.75344915e-01, 1.07420563e-01, 1.74862017e-01, 1.74862017e-01, 2.60039550e-01, 4.60318116e-01, 9.72879449e-01, 1.58549846e+00, 1.58549846e+00,
            1.16101810e+00, 5.44278482e-01, 1.11328067e+00, 1.38688761e+00, 1.38688761e+00, 7.60243090e-01, 9.55639062e-01, 4.93582505e-01, 1.29068740e+00, 1.29068740e+00,
            5.28689564e-01, 1.39381110e+00, 5.42677302e-01, 3.96145861e-01, 3.96145861e-01, 8.62658725e-01, 1.36815016e+00, 6.22753674e-01, 3.82362694e-01, 3.82362694e-01,
            6.30436264e-01, 1.20538505e+00, 3.49684587e-01, 4.22038244e-01, 6.14216446e-01, 6.14216446e-01, 1.02476568e+00, 1.27447365e+00, 1.90321291e+00, 1.21478525e-01,
            1.21478525e-01, 4.30550247e-02, 3.90404052e-02, 1.05524063e-01, 3.01278475e-02, 2.85022864e-02, 2.85022864e-02, 9.15452760e-02, 3.50578957e-02, 8.56477077e-02,
            8.30080400e-02, 8.30080400e-02, 5.49600160e-02, 7.85257788e-02, 4.86958723e-02, 6.65462596e-02, 1.45430943e-01, 1.45430943e-01, 5.45882259e-02, 4.34156119e-02,
            3.14234949e-02, 1.45931740e-02, 1.45931740e-02, 2.15276166e-02, 2.63384040e-02, 6.53243030e-02, 1.03271350e-01, 1.03271350e-01, 1.72650701e-01, 7.72907307e-02,
            2.51738210e-02, 3.36033118e-02, 3.36033118e-02, 5.14876804e-02, 2.98471961e-02, 1.69890222e-02, 3.44701632e-02, 5.12670870e-02, 5.12670870e-02, 5.17475716e-02,
            7.74148953e-02, 2.08834189e-02, 2.31803762e-02, 2.31803762e-02, 7.08278513e-02, 2.64161680e-02, 7.15087576e-02, 1.48712950e-02, 3.95790413e-02, 3.95790413e-02,
            2.91665991e-02, 9.94305229e-03, 1.35623868e-02, 2.15165949e-02, 2.15165949e-02, 3.28404454e-02, 6.81245571e-02, 1.38691163e-01, 6.73318216e-02, 6.73318216e-02,
            2.35001096e-02, 3.78024741e-02, 4.79717189e-02, 2.80124407e-02, 2.80124407e-02, 1.63597420e-02, 3.01193641e-02, 4.40313460e-02, 4.64430246e-02, 8.72137603e-02,
            8.72137603e-02, 1.69259657e-02, 2.47814804e-02, 6.75801010e-02, 2.76370353e-02, 2.76370353e-02, 6.83906505e-02, 1.51202396e-02, 4.22252578e-02, 2.74684955e-02,
            2.74684955e-02, 9.97615164e-03, 1.40308366e-02, 1.95477777e-02, 3.82525995e-02, 7.21736452e-02, 7.21736452e-02, 1.44561140e-01, 9.95461426e-02, 3.24164968e-02,
            8.17408882e-02, 8.17408882e-02, 1.14204226e-01, 5.95120011e-02, 5.68381770e-02, 3.37099723e-02, 3.37099723e-02, 1.15838484e-01, 3.49398490e-02, 1.12824599e-01,
            4.51684337e-02, 3.15778804e-02, 3.15778804e-02, 1.71884125e-01, 9.68154612e-02, 4.09810704e-02, 2.43571445e-02, 2.43571445e-02, 3.68115331e-02, 1.27374994e-01,
            2.18803324e-02, 3.21351970e-02, 3.21351970e-02, 4.25589350e-02, 7.04814410e-02, 1.21360409e-01, 2.31756712e-01, 2.31756712e-01, 5.91792014e-03, 3.61098267e-03,
            3.33312062e-03, 5.75663533e-03, 2.95032108e-03, 2.95032108e-03, 2.73400376e-03, 5.11506322e-03, 3.12312335e-03, 4.91920469e-03, 4.91920469e-03, 4.71668004e-03,
            4.78762221e-03, 5.26716829e-03, 4.04768289e-03, 4.04768289e-03, 4.63094050e-03, 5.06507711e-03, 4.09466717e-03, 3.77963538e-03, 3.33594903e-03, 3.33594903e-03,
            1.64804062e-03, 2.21494032e-03, 2.96516616e-03, 4.47991667e-03, 4.47991667e-03, 5.45484273e-03, 7.35123118e-03, 4.40597894e-03, 2.65318284e-03, 2.65318284e-03,
            2.99336225e-03, 3.64401338e-03, 2.34371665e-03, 1.99446415e-03, 1.99446415e-03, 2.82374028e-03, 3.54000659e-03, 4.04782976e-03, 4.51235634e-03, 2.32907982e-03,
            2.32907982e-03, 2.65953673e-03, 4.53387095e-03, 1.89096827e-03, 4.41246910e-03, 4.41246910e-03, 1.66689991e-03, 3.53279411e-03, 2.81853833e-03, 1.09309390e-03,
            1.61499692e-03, 1.61499692e-03, 2.10962142e-03, 3.29549620e-03, 4.68525136e-03, 6.17703981e-03, 6.17703981e-03, 4.82972536e-03, 2.24422459e-03, 2.88123492e-03,
            3.97024693e-03, 2.51682834e-03, 2.51682834e-03, 1.81090501e-03, 3.09472601e-03, 3.68069817e-03, 3.24575865e-03, 3.24575865e-03, 4.90825379e-03, 1.79681753e-03,
            2.21258160e-03, 3.00013843e-03, 3.00013843e-03, 2.50764118e-03, 3.62974559e-03, 1.44011638e-03, 3.23115392e-03, 2.79760067e-03, 2.79760067e-03, 1.14403951e-03,
            1.52787192e-03, 1.98912083e-03, 3.25091211e-03, 3.25091211e-03, 4.24110612e-03, 6.39316497e-03, 5.39633241e-03, 3.25289106e-03, 3.25289106e-03, 5.46943934e-03,
            6.22810235e-03, 4.55609678e-03, 4.70978500e-03, 3.14368373e-03, 3.14368373e-03, 6.28409754e-03, 3.49194702e-03, 5.71074589e-03, 3.72204546e-03, 3.72204546e-03,
            2.97219302e-03, 8.00145107e-03, 3.66055796e-03, 3.55345458e-03, 2.47647733e-03, 2.47647733e-03, 3.43998037e-03, 6.73941318e-03, 2.29181176e-03, 3.01756382e-03,
            3.01756382e-03, 3.68889955e-03, 4.72598168e-03, 6.20442573e-03, 7.34856160e-03, 7.34856160e-03 };

        std::vector<double> gains = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 16,
            16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
            16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
            16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
            16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
            16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
            16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
            16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
            16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
            16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
            16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
            16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
            16, 16, 16, 16, 16, 16, 16, 16, 128, 128,
            128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
            128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
            128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
            128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
            128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
            128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
            128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
            128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
            128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
            128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
            128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
            128, 128, 128, 128, 128, 128 };

        // TODO: This does compile on Linux but aborts at runtime.
        // Originally LIBLAB models were supported on Windows only,
        // but naturally it would be great to fix this also.
        #ifdef _MSC_VER
            THEN("noise model calculation should return successfully")
            {
                // y = c1*x^2 + AG*c2*x + AG^2*c3 + AG*c4 + c5
                double c1 = 0.000067528625358;
                double c2 = 0.103974649088652;
                double c3 = 0.044852567264030;
                double c4 = 0.094757673083903;
                double c5 = 1.655897545970105;

                Teisko::noise_model noise_model;

                auto res = Teisko::calculate_unified_non_neg_noise_model(means, vars, weights, gains, noise_model);

                CHECK(Teisko::NoiseModel::SUCCESS == res);

                AND_THEN("noise model result should be close to reference values")
                {
                    CHECK(c1 == Approx(noise_model.c1).epsilon(1e-8));
                    CHECK(c2 == Approx(noise_model.c2).epsilon(1e-8));
                    CHECK(c3 == Approx(noise_model.c3).epsilon(1e-8));
                    CHECK(c4 == Approx(noise_model.c4).epsilon(1e-8));
                    CHECK(c5 == Approx(noise_model.c5).epsilon(1e-8));
                }
            }
        #endif
    }
}