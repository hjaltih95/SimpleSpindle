/* -------------------------------------------------------------------------- *
*                         OpenSim:  mainSpindle.cpp                          *
* -------------------------------------------------------------------------- *
* The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
* See http://opensim.stanford.edu and the NOTICE file for more information.  *
* OpenSim is developed at Stanford University and supported by the US        *
* National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
* through the Warrior Web program.                                           *
*                                                                            *
* Copyright (c) 2005-2017 Stanford University and the Authors                *
* Author(s): Peter Loan, Ajay Seth, Ayman Habib                              *
*                                                                            *
* Licensed under the Apache License, Version 2.0 (the "License"); you may    *
* not use this file except in compliance with the License. You may obtain a  *
* copy of the License at http://www.apache.org/licenses/LICENSE-2.0.         *
*                                                                            *
* Unless required by applicable law or agreed to in writing, software        *
* distributed under the License is distributed on an "AS IS" BASIS,          *
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied    *
* See the License for the specific language governing permissions and        *
* limitations under the License.                                             *
* -------------------------------------------------------------------------- */

//=============================================================================
//=============================================================================
#include <OpenSim/OpenSim.h>
#include "SimpleSpindle.h"
#include <OpenSim/Common/IO.h>
#include "OpenSim/Common/STOFileAdapter.h"

using namespace OpenSim;
using namespace SimTK;

//_____________________________________________________________________________
/**
 * Run a simulation of a sliding block being pulled by two muscle
 */

int main() {
    
    std::clock_t startTime = std::clock();
    
    try {
        ///////////////////////////////////////////////
        // DEFINE THE SIMULATION START AND END TIMES //
        ///////////////////////////////////////////////
        // Define the initial and final simulation times
        double initialTime = 0.0;
        double finalTime = 10.0;
        
        ///////////////////////////////////////////
        // DEFINE BODIES AND JOINTS OF THE MODEL //
        ///////////////////////////////////////////
        // Create an OpenSim model and set its name
        Model osimModel;
        osimModel.setName("tugofWar");
        
        // GROUND FRAME
        
        // Get a reference to the model's ground body
        Ground& ground = osimModel.updGround();
        
        // Add display geometry to the ground to visualize in the GUI
        ground.attachGeometry(new Mesh("ground.vtp"));
        ground.attachGeometry(new Mesh("anchor1.vtp"));
        ground.attachGeometry(new Mesh("anchor2.vtp"));
        
        // BLOCK BODY
        
        // Specify properties of a 20 kg 1cm length block body
        double blockMass = 20.0, blockSideLength = 0.1;
        Vec3 blockMassCenter(0);
        Inertia blockInertia = blockMass*Inertia::brick(blockSideLength, blockSideLength, blockSideLength);
        
        // Create a new block body with the specified properties
        OpenSim::Body *block = new OpenSim::Body("block", blockMass, blockMassCenter, blockInertia);
        
        block->attachGeometry(new Mesh("block.vtb"));
        
        // FREE JOINT
        
        // Create a new free joint with 6 degrees-of-freedom (coordinates)
        // between the block and ground bodies
        double halfLength = blockSideLength/2.0;
        Vec3 locationInParent(0, halfLength, 0), orientationInParent(0);
        Vec3 locationInBody(0, halfLength, 0), orientationInBody(0);
        FreeJoint *blockToGround = new FreeJoint("blockToGround", ground, locationInParent, orientationInParent, *block, locationInBody, orientationInBody);
        
        // Set the angle and position ranges for the free (6-degree-of-freedom)
        // joint between the block and ground frames.
        double angleRange[2] = {-SimTK::Pi/2, SimTK::Pi/2};
        double positionRange[2] = {-1, 1};
        blockToGround->updCoordinate(FreeJoint::Coord::Rotation1X).setRange(angleRange);
        blockToGround->updCoordinate(FreeJoint::Coord::Rotation2Y).setRange(angleRange);
        blockToGround->updCoordinate(FreeJoint::Coord::Rotation3Z).setRange(angleRange);
        blockToGround->updCoordinate(FreeJoint::Coord::TranslationX).setRange(positionRange);
        blockToGround->updCoordinate(FreeJoint::Coord::TranslationY).setRange(positionRange);
        blockToGround->updCoordinate(FreeJoint::Coord::TranslationZ).setRange(positionRange);
        
        // Add the block body to the model
        osimModel.addBody(block);
        osimModel.addJoint(blockToGround);
        
        ///////////////////////////////////////
        // DEFINE FORCES ACTING ON THE MODEL //
        ///////////////////////////////////////
        // MUSCLE FORCES
        // Create two new muscles
        double maxIsometricForce = 1000.0, optimalFiberLength = 0.2,
        tendonSlackLength = 0.1,    pennationAngle = 0.0;
        
        // muscle models
         Millard2012EquilibriumMuscle* original1 =
             new Millard2012EquilibriumMuscle("original1",
                 maxIsometricForce, optimalFiberLength, tendonSlackLength,
                 pennationAngle);
        
        Millard2012EquilibriumMuscle* original2 =
            new Millard2012EquilibriumMuscle("original2",
                maxIsometricForce, optimalFiberLength, tendonSlackLength,
                pennationAngle);
        
        // Define the path of the muscles
        original1->addNewPathPoint("original1-point1", ground,
            Vec3(0.0, halfLength, 0.35));
        original1->addNewPathPoint("original1-point2", *block,
            Vec3(0.0, halfLength, halfLength));
        
        original2->addNewPathPoint("original2-point1", ground,
            Vec3(0.0, halfLength, 0.35));
        original2->addNewPathPoint("original2-point2", *block,
            Vec3(0.0, halfLength, halfLength));
        
        // Define the default states for the two muscles
        // Activation
        original1->setDefaultActivation(0.01);
        original2->setDefaultActivation(0.01);
        
        // Fiber length
        original1->setDefaultFiberLength(optimalFiberLength);
        original2->setDefaultFiberLength(optimalFiberLength);
        
        // Add the two muscles to the model
        osimModel.addForce(original1);
        osimModel.addForce(original2);
        
        ///////////////////////////////////
        // DEFINE CONTROLS FOR THE MODEL //
        ///////////////////////////////////
        

        
        
        
        
        
        
        
        
        
    }
    
    catch(const std::exception& ex){
        std::cout << ex.what() << std::endl;
        return 1;
    }
    
    catch(...){
        std::cout << "UNRECOGNIZED EXCEPTION" << std::endl;
        return 1;
    }
    
    std::cout << "main() routine time = " << 1.e3*(std::clock()-startTime)/CLOCKS_PER_SEC << "ms\n";
    
    std::cout << "OpenSim simulation completed successfully.\n";
    
    
    return 0;
}
