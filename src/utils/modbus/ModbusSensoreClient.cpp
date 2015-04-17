#include "ModbusSensoreClient.h"

namespace modbus{

void ModbusSensorClientInterface::ProcessSensorCallback(){
    std::cout<< "called sensore callback, PLEASE REDEFINE" << std::endl;
}
void ModbusSensorClientInterface::ProcessBeaconCallback(){
    std::cout<< "called beckon callback, PLEASE REDEFINE" << std::endl;
}

void ModbusSensorClientInterface::ProcessEnemySensorCallback1(){
    std::cout<< "called sensore callback, PLEASE REDEFINE" << std::endl;
}

void ModbusSensorClientInterface::ProcessEnemySensorCallback2(){
    std::cout<< "called sensore callback, PLEASE REDEFINE" << std::endl;
}

void ModbusSensorClientInterface::ProcessEnemySensorCallback3(){
    std::cout<< "called sensore callback, PLEASE REDEFINE" << std::endl;
}

void ModbusSensorClientInterface::ProcessEnemySensorCallback4(){
    std::cout<< "called sensore callback, PLEASE REDEFINE" << std::endl;
}

bool operator< (const idData &id1, const idData &id2){
    if((id1.functionAddress*id1.slaveAddress)< (id2.functionAddress * id2.slaveAddress)) return true;
    else return false;
}


ModbusSensorClient* ModbusSensorClient::instance = 0;

ModbusSensorClient::ModbusSensorClient(): Node("ModbusServoClient"),m_mutex(new boost::mutex()){
    modbus = ModbusClient::getMobusClientInstance();
    start();
    std::cout << "ModbusSensoreClient is runnging" << std::endl;
    ModbusSensoreClientNotifier = modbus->getModbusSensorNotifier();
    respinNumber = int(2000/ delayTime);
    brxonID.slaveAddress = char(4);
    brxonID.functionAddress = (char(1));
    brxonStartID.slaveAddress = char(4);
    brxonStartID.functionAddress = char(8);

    beaconID.slaveAddress = char(2);
    beaconID.functionAddress_Start = char(1);
    beaconID.functionAddress_NumberOfBeacons = char(2);
    beaconID.functionAddress_XdataBecaon1 = char(1);
    beaconID.functionAddress_YdataBeacon1 = char(2);
    beaconID.functionAddress_XdataBecaon2 = char(3);
    beaconID.functionAddress_YdataBeacon2 = char(4);
    beaconID.beaconON = false;

}

ModbusSensorClient* ModbusSensorClient::getModbusSensorInstance(){
    if (!instance){
        instance = new ModbusSensorClient();
    }
    return instance;
}


void ModbusSensorClient::registerToSensoreCallback(unsigned char _slave_address, unsigned short _coil_address, bool on_bool_callback,
                                                   ModbusSensorClientInterface *obj){

    boost::lock_guard<boost::mutex> lock(*m_mutex);

    idData id;
    id.slaveAddress = _slave_address;
    id.functionAddress = _coil_address;
    callbackRegisterMap[id] =(ModbusSensorClientInterface*) obj;
}

void ModbusSensorClient::main(){
    //bool success = false;
    bool doCallbak = false;
    bool recalculate = false;
    timerForDelayTime = 0;

    while(!shouldStop){

        for(it_type it = callbackRegisterMap.begin(); it != callbackRegisterMap.end(); ++it){

            m_mutex->lock();
            recalculate = *ModbusSensoreClientNotifier;
            m_mutex->unlock();

            if(recalculate && !delayAlreadyCalculated){
                delayAlreadyCalculated = true;
                delayTime = delayTime2;
                std::cout << "sensore Modbus slowing down " << std::endl;
            }

            //std::cout << timerForDelayTime << " : " << respinNumber << std::endl;
            //std::cout << "delayTime" << delayTime << std::endl;

            if(timerForDelayTime < respinNumber && didReading){
                timerForDelayTime ++;
                //std::cout << "counter++ " << std::endl;
            }else{
                // std::cout << "end of counter " << std::endl;
                delayTime = delayTime1;
                delayAlreadyCalculated = false;
                timerForDelayTime = 0;

                m_mutex->lock();
                *ModbusSensoreClientNotifier = false;
                m_mutex->unlock();
                //std::cout << "sensor Modbus speed boosting" << std::endl;

            }

            if(beaconReading > 10 && beaconID.beaconON){
                beaconReading = 0;
                std::cout << " reading beacon" << std::endl;
                readBeacon();
                boost::this_thread::sleep(boost::posix_time::milliseconds(delayTime));
            }else if(beaconID.beaconON){
                beaconReading++;
            }


            if(it->second->readingSensore == true){
                modbus->readCoil(&doCallbak, it->first);
                //std::cout << "reading coil" << std::endl;
                if (doCallbak){
                    it->second->readingSensore = false;
                    switch(it->first.functionAddress){
                    //case (char(1)): it->second->ProcessEnemySensorCallback1(); std::cout << "enemy 1" << std::cout; break;
                    //case (char(2)): it->second->ProcessEnemySensorCallback2(); std::cout << "enemy 1" << std::cout; break;
                    //case (char(5)): it->second->ProcessEnemySensorCallback3(); std::cout << "enemy 1" << std::cout; break;
                    case 1: it->second->ProcessEnemySensorCallback1(); break;
                    case 2: it->second->ProcessEnemySensorCallback2(); break;
                    case 5: it->second->ProcessEnemySensorCallback3(); break;
                    case 7: it->second->ProcessEnemySensorCallback4(); break;
                    default: it->second->ProcessSensorCallback();
                    }
                }
                didReading = true;

                boost::this_thread::sleep(boost::posix_time::milliseconds(delayTime));
            }
            //didReading = true;
            //boost::this_thread::sleep(boost::posix_time::milliseconds(delayTime));
        }
    }
}

void ModbusSensorClient::stopModbusSensorClient(){
    shouldStop = true;
}

short ModbusSensorClient::readBrxon(){
    short data;
    bool success;
    success = modbus->readRegister(&data, brxonID.slaveAddress, brxonID.functionAddress);
    return data;
}

bool ModbusSensorClient::startBrxon(){
    modbus->setCoil(brxonStartID.slaveAddress, brxonStartID.functionAddress, 1);
    return true;
}

bool ModbusSensorClient::stopBrxon(){
    modbus->setCoil(brxonStartID.slaveAddress, brxonStartID.functionAddress, 0);
    return true;
}

bool ModbusSensorClient::startBeacon(){
    bool success = false;
    if(!beaconID.beaconON){

        success =  modbus->setCoil(beaconID.slaveAddress, beaconID.functionAddress_NumberOfBeacons, 0 );
        success =  modbus->setCoil(beaconID.slaveAddress, beaconID.functionAddress_Start, 1 );

        if(success){
            beaconID.beaconON = true;
            std::cout << " Beacon is spping " << std::endl;
        }else{
            std::cout << " Beacon ERROR" << std::endl;
        }
    }
    return success;
}

bool ModbusSensorClient::stopBecaon(){
    bool success = false;
    if(beaconID.interface != NULL){
        modbus->setCoil(beaconID.slaveAddress, beaconID.functionAddress_Start, 0);
        if(success) beaconID.beaconON = false;
    }else{
        std::cout<< "have to register to beacon interface firts";
    }
    return false;
}

bool ModbusSensorClient::readBeacon(){
    short data;
    bool success;
    switch (beaconID.beaconDataCounter){
    case 0: {
        success = modbus->readRegister(&data ,beaconID.slaveAddress, beaconID.functionAddress_XdataBecaon1);
        if(success){
            beaconID.beaconData.X_beacon1 = data;
            beaconID.beaconDataCounter++;
        }
        //break;
    }
    case 1:{
        success = modbus->readRegister(&data ,beaconID.slaveAddress, beaconID.functionAddress_YdataBeacon1);
        if(success){
            beaconID.beaconData.Y_beacon1 = data;
            beaconID.beaconDataCounter++;
        }
        //break;
    }
    case 2:{
        success = modbus->readRegister(&data ,beaconID.slaveAddress, beaconID.functionAddress_XdataBecaon2);
        if(success){
            beaconID.beaconData.X_beacon1 = data;
            beaconID.beaconDataCounter++;
        }
        break;
    }
    case 3:{
        success = modbus->readRegister(&data ,beaconID.slaveAddress, beaconID.functionAddress_YdataBeacon2);
        if(success){
            beaconID.beaconData.X_beacon1 = data;
            beaconID.beaconDataCounter = 0;
            beaconID.interface->beaconData.X_beacon1 = beaconID.beaconData.X_beacon1;
            beaconID.interface->beaconData.Y_beacon1 = beaconID.beaconData.Y_beacon1;
            beaconID.interface->beaconData.X_beacon2 = beaconID.beaconData.X_beacon2;
            beaconID.interface->beaconData.Y_beacon1 = beaconID.beaconData.Y_beacon2;
            beaconID.interface->ProcessBeaconCallback();
            std::cout << "BEACON WRITE DATA" << std::endl;
        }
        //break;
    }
    } // end switch

    return success;
}

void ModbusSensorClient::registerToBeaconInterface(ModbusSensorClientInterface* _interface){
    std::cout << "registred to Beacon Interface" << std::endl;
    beaconID.interface = _interface;
}

} // end namespace modbus
