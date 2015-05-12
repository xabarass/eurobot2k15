#include "EnemyDetector.h"

namespace executor{

std::string EnemyDetector::NAME = "EnemyDetectorExecutor";


void EnemyDetector::suscribe(){

    this->registerCommand(ActuatorCommand::NAME, static_cast<commandCallback>(&EnemyDetector::processActuatorCommand));

#ifdef VELIKI_ROBOT
    frontSensor.setConfig(char(4),char(1),sensorFrontID,this, true);
    frontSensor.RegisterSensor();
    frontSensor.StartSensor();

    backSensor.setConfig(char(4), char(5),sensorBackID, this, true);
    backSensor.RegisterSensor();
    backSensor.StartSensor();

    brkon.setCoilConfig(char(4),char(7));
    brkon.setRegisterConfig(char(4),char(1));
    brkon.setPowerCoilConfig(char(4),char(8));
    brkon.registerInerface(this);
    brkon.startBrkon();

    //TODO
    //dodati za beacon
    beacon.setBeaconConfig(char(4),char(1),char(2),char(4),char(3),char(4),char(4),char(1),char(2),2,this);
    beacon.registerBeacon();
    beacon.startBeacon();


#endif

#ifdef MALI_ROBOT

#endif

}

bool EnemyDetector::StartBrxonFunction(){
    debug("Starting Brkon");
    brkon.startBrkon();
    return true;
}

bool EnemyDetector::StopBrxonFunction(){
    debug("stopBrkon");
    brkon.stopBrkon();
    return true;
}

void EnemyDetector::SensorDriverCallback(int _id, bool _detected){
    /*
    //debug("SENSOR CALLBACK ");
    if(_id == this->sensorBackID){
        if(previousState.sensorBack != _detected){
            previousState.sensorBack = _detected;

            if(_detected){
                debug("DOSO BACK");
            }else{
                debug("OTISAO BACK");
            }
        }
        backSensor.StartSensor();
    }else if(_id ==this->sensorFrontID){
        if(previousState.sensorFront != _detected){
            previousState.sensorFront = _detected;

            if(_detected){
                debug("DOSO FRONT");
            }else{
                debug("OTISAO FRONT");
            }
        }
        frontSensor.StartSensor();
    }else{
        debug("WROOONG ID ");
    }
    */
    SensorCommand* command = new SensorCommand(_id,_detected);
    commandQueueLock.lock();
    commandsToProcess.push(Instruction(command));
    commandQueueLock.unlock();
    queueNotEmpty.notify_one();

}

void EnemyDetector::brkonDriverCallback(short _dataFront, short _dataBack, bool _detected){
    /*
    std::cout << "angle front: " << _dataFront << std::endl
              << "angle back: " << _dataBack << std::endl;
              */
    BrkonCommand* command = new BrkonCommand(_dataFront, _dataBack, _detected);
    commandQueueLock.lock();
    commandsToProcess.push(Instruction(command));
    commandQueueLock.unlock();
    queueNotEmpty.notify_one();

}

void EnemyDetector::beaconDriverCallbackVeliki(short _veliki_cordX, short _veliki_cordY, bool _running){
    if(_running){
        // when beacon is running and getting valid data
        std::cout << "beacon VELIKI X: "
                  << _veliki_cordX << " Y: "
                  << _veliki_cordY << std::endl;
    }else{
        debug("beacon is not getting valide data");
    }
    BeaconVelikiCommand* command = new BeaconVelikiCommand(_veliki_cordX,_veliki_cordY, _running);

    commandQueueLock.lock();
    commandsToProcess.push(Instruction(command));
    commandQueueLock.unlock();
    queueNotEmpty.notify_one();
}


void EnemyDetector::beaconDriverCallbackMali(short _mali_cordX, short _mali_cordY, bool _running){
    /*
    if(_running){
        // when beacon is running and getting valid data
        std::cout << "beacon MALI X: "
                  << _mali_cordX << " Y: "
                  << _mali_cordY << std::endl;
    }else{
        debug("beacon is not getting valide data");
    }
    */
    BeaconMaliCommand* command = new BeaconMaliCommand(_mali_cordX,_mali_cordY, _running);

    commandQueueLock.lock();
    commandsToProcess.push(Instruction(command));
    commandQueueLock.unlock();
    queueNotEmpty.notify_one();

}

bool EnemyDetector::StopBeaconFunction(){
    beacon.stopBeacon();
    return true;
}

bool EnemyDetector::StartBeaconFunction(){
    beacon.startBeacon();
    return true;
}

bool EnemyDetector::BeaconMaliCallbackFunction(short _cordX, short _cordY, bool _running){
    if(_running){
        std::cout << "X: " << _cordX << " Y: " << _cordY << std::endl;
    }
    return true;
}

bool EnemyDetector::BeaconVelikiCallbackFunction(short _cordX, short _cordY, bool _running){
    if(_running){
        std::cout << "X: " << _cordX << " Y: " << _cordY << std::endl;
    }
    return true;
}

bool EnemyDetector::BrkonCallbackFunction(short _dataFront, short _dataBack, bool _detected){
    if(_detected){
    std::cout << "angle front: " << _dataFront << std::endl
              << "angle back: " << _dataBack << std::endl;

    }else{

    }
    return true;
}

bool EnemyDetector::SensorCallbackFunction(int _id, bool _detected){
EnemyDetectedNotification* notification=NULL;    
if(_id == this->sensorBackID){
        if(previousState.sensorBack != _detected){
            previousState.sensorBack = _detected;

            if(_detected){
                notification=new EnemyDetectedNotification(EnemyDetectedNotification::Type::BACK,0,true);
                debug("DOSO BACK");
            }else{
                notification=new EnemyDetectedNotification(EnemyDetectedNotification::Type::BACK,0,false);
                debug("OTISAO BACK");
            }
        }
        backSensor.StartSensor();
    }else if(_id ==this->sensorFrontID){
        if(previousState.sensorFront != _detected){
            previousState.sensorFront = _detected;

            if(_detected){
                notification=new EnemyDetectedNotification(EnemyDetectedNotification::Type::FRONT,0,true);
                debug("DOSO FRONT");
            }else{
                notification=new EnemyDetectedNotification(EnemyDetectedNotification::Type::FRONT,0,false);
                debug("OTISAO FRONT");
            }
        }
        frontSensor.StartSensor();
    }else{
        debug("WROOONG ID ");
    }
    if (notification!=NULL){
        debug("Sending enemy detected notification!");
        sendNotification(notification);
    }
    return true;
}


} // end namespace
