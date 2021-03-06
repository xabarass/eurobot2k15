#include "ModbusClient.h"

namespace modbus{

void ModbusClientCallbackInterface::modbusEventReceived(){

}

ModbusClient* ModbusClient::instance=0;

ModbusClient::ModbusClient():Node("ModbusClient"),m_mutex(new boost::mutex()){
    modbus = ModbusMaster::getModbusInstance();
    start();
}

ModbusClient* ModbusClient::getMobusClientInstance(){
    if(!instance){
        instance = new modbus::ModbusClient();
    }
    return instance;
}


void ModbusClient::main(){

    while(!shouldStop){
       /* **********************************************************************************
        if(panic){
            //std::cout << "PANIC !! ELECTRONIC IS NOT WORKING" << std::endl;
            while(!registersToSet.empty()) registersToSet.pop();
            while(!coilToSet.empty()) coilToSet.pop();
            return;
        }
        */
        Instruction inst = getNextInstruction();
       switch(inst.instruction){
       case SET_REGISTER: writeToRegister(inst.id_data); break;
       case SET_COIL: writeToCoil(inst.id_data); break;
       case STOP: return;
       }

    }
}


ModbusClient::Instruction ModbusClient::getNextInstruction(){
    unique_lock<boost::mutex> lock(InstructionQueueMutex);
    while(InstructionQueue.empty()){
        queueNotEmpty.wait(lock);
    }
    Instruction inst;
    inst = InstructionQueue.front();
    InstructionQueue.pop();

    return inst;

}

/*
bool ModbusClient::setRegister(unsigned char _slave_address, short _function_address, short _data ){ // za klienta
    setSingleRegisterData data;
    //Instruction inst;


    data.ID.slaveAddress = _slave_address;
    data.ID.functionAddress = _function_address;
    data.data = _data;

    //inst.insctruction = SET_REGISTER;
    //inst.id_data = data;


    queueLock.lock();
    InstructionQueue.push(Instruction(SET_REGISTER, data));
    queueLock.unlock();
    queueNotEmpty.notify_one();

    return true;
}

bool ModbusClient::setCoil(unsigned char _slave_address, short _function_address, short _data ){

    setSingleRegisterData data;

    data.ID.slaveAddress = _slave_address;
    data.ID.functionAddress = _function_address;
    data.data = _data;

    queueLock.lock();
    InstructionQueue.push(Instruction(SET_COIL,data));
    queueLock.unlock();
    queueNotEmpty.notify_one();
    return true;

}
zamenjeno sa blocking and nonBlocking
*/


bool ModbusClient::writeToRegister(setSingleRegisterData data){
    bool success;
    int counter = 0;


    std::cout << "writeing to register: "
              << int(data.ID.slaveAddress) << ":"
              << data.ID.functionAddress << ":"
              << data.data << std::endl;

        boost::lock_guard<boost::mutex> lock(*m_mutex);

        success = modbus->ModbusPresetSingleRegister(data.ID.slaveAddress, data.ID.functionAddress, data.data);

        while(!success && !shouldStop && counter < 10 ) {
            boost::this_thread::sleep(boost::posix_time::milliseconds(delayTime));
            success = modbus->ModbusPresetSingleRegister(data.ID.slaveAddress, data.ID.functionAddress, data.data);
            counter ++;
        }
        if(counter > 9){
            std::cout << "PANIC ELECTRONIC IS NOT WORKING" << std::endl;
//            panic = true;   //***************************************************************************
         }

        counter = 0;
        return success;

}

bool ModbusClient::writeToCoil(setSingleRegisterData data){
    bool success;
    int counter = 0;

        std::cout << "writeing to coil: "
                  << int(data.ID.slaveAddress) << ":"
                  << data.ID.functionAddress << ":"
                  << data.data << std::endl;

        boost::lock_guard<boost::mutex> lock(*m_mutex);

        success = modbus->ModbusForceSingleCoil(data.ID.slaveAddress, data.ID.functionAddress, data.data);

        while(!success && !shouldStop && counter < 10){
            boost::this_thread::sleep(boost::posix_time::milliseconds(delayTime));
            success = modbus->ModbusForceSingleCoil(data.ID.slaveAddress, data.ID.functionAddress, data.data);
            counter ++;
        }

        if(counter > 9 ){
            std::cout << "PANIC ELECTRONIC IS NOT WORKING" << std::endl;
            panic = true; // **************************************************************************
        }

        counter = 0;
        return success;

}

bool ModbusClient::setCoil(unsigned char _slave_address, short _function_address, short _data, bool _blocking){
    setSingleRegisterData data;

    data.ID.slaveAddress = _slave_address;
    data.ID.functionAddress = _function_address;
    data.data = _data;

    if(!_blocking){
        queueLock.lock();
        InstructionQueue.push(Instruction(SET_COIL,data));
        queueLock.unlock();
        queueNotEmpty.notify_one();
        return true;
    }else{
       return writeToCoil(data);
    }
}

bool ModbusClient::setRegister(unsigned char _slave_address, short _function_address, short _data, bool _blocking){
    setSingleRegisterData data;

    data.ID.slaveAddress = _slave_address;
    data.ID.functionAddress = _function_address;
    data.data = _data;

    if(!_blocking){

        queueLock.lock();
        InstructionQueue.push(Instruction(SET_REGISTER, data));
        queueLock.unlock();
        queueNotEmpty.notify_one();

        return true;

    }else{
        return writeToRegister(data);
    }
}


bool ModbusClient::readCoil(bool* _callFunction, idData _id ){
    signed char data;
    boost::lock_guard<boost::mutex> lock(*m_mutex);

    if(!panic){

        bool success = modbus->ModbusReadCoilStatus(_id.slaveAddress,_id.functionAddress,1, &data);

        while(!success && !shouldStop && counter < 10 ) {
            boost::this_thread::sleep(boost::posix_time::milliseconds(delayTime));
            success = modbus->ModbusReadCoilStatus(_id.slaveAddress,_id.functionAddress,1, &data);
            counter++;
        }

        if(counter > 9){
            std::cout << "PANIC ELECTRONIC IS NOT WORKING" << std::endl;
            //panic = true;  // ****************************************************************
        }

        counter = 0;

        /*
    std::cout << "reading from register: "
              << int(_id.slaveAddress) << " : "
              << _id.functionAddress << " = "
              << int(data) << " : "
              << data
              << std::endl;
*/
        if (!success){
            *_callFunction = false;
            return false;
        }

        if(data == char(1)){
            *_callFunction = true;
            //std::cout << "OBJECT" << std::endl;
        }else if (data == '1'){
            *_callFunction = true;
            //std::cout << "OBJECT" << std::endl;
        }else if(data == char(0)){
            *_callFunction = false;
        }else if(data == '0'){
            *_callFunction = false;
        }else{
            std::cout << "ERRORO IN CHECKING STATE" << std::endl;
            *_callFunction = false;
        }

        return true;
    }
    return false;
}

void ModbusClient::stopModbusClient(){
    setSingleRegisterData data;
    queueLock.lock();
    InstructionQueue.push(Instruction(STOP, data));
    queueLock.unlock();
    queueNotEmpty.notify_one();
    shouldStop = true;
}

bool* ModbusClient::getModbusSensorNotifier(){
    boost::lock_guard<boost::mutex> lock(*m_mutex);
    return &ModbusSensorClientNotifier;

}

bool* ModbusClient::getModbusSensorPanic(){
    boost::lock_guard<boost::mutex> lock(*m_mutex);
    return &panic;
}


bool ModbusClient::readRegister(short* _data, unsigned char _slaveAddress, short _functionAddress){
    short data;
    bool success;
    int counter = 0;

    if (!panic){

        boost::lock_guard<boost::mutex> lock(*m_mutex);

        success = modbus->ModbusReadHoldingRegisters(_slaveAddress,_functionAddress,1,&data);

        while(!success && !shouldStop && counter < 10 ) {
            boost::this_thread::sleep(boost::posix_time::milliseconds(delayTime));
            success = modbus->ModbusReadHoldingRegisters(_slaveAddress,_functionAddress,1,&data);
            counter++;
        }

        if(counter > 9){
            std::cout << "PANIC ELECTRONIC IS NOT WORKING" << std::endl;
            panic = true;
        }

        *_data = data;
        return success;
    }
    return false;
}

bool ModbusClient::getPanic(){
    return panic;
}

} // end namespace
