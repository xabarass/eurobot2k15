#include "ExecutorCommon.h"

namespace executor{

void ExecutorCommon::init(){

    actuatorHandles[ActuatorType::RELOAD_CONFIG]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::reloadConfig);
    actuatorHandles[ActuatorType::GET_POPCORN]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::getPopcorn);
    actuatorHandles[ActuatorType::UNLOAD_POPCORN]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::unloadPopcorn);
    actuatorHandles[ActuatorType::KICK_RIGHT]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::kickRight);
    actuatorHandles[ActuatorType::KICK_LEFT]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::kickLeft);
    actuatorHandles[ActuatorType::UNKICK_RIGHT]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::unKickRight);
    actuatorHandles[ActuatorType::UNKICK_LEFT]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::unKickLeft);
    actuatorHandles[ActuatorType::GET_OBJECT]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::getObject);
    actuatorHandles[ActuatorType::UNLOAD_OBJECT]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::unloadObject);
    actuatorHandles[ActuatorType::RELOAD_CONFIG]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::reloadConfig);
    actuatorHandles[ActuatorType::GET_OBJECT_STOP]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::getObjectStop);
    actuatorHandles[ActuatorType::START_BRXON]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::startBrxon);
    actuatorHandles[ActuatorType::STOP_BRXON]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::stopBrxon);
    actuatorHandles[ActuatorType::START_BEACON]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::startBeacon);
    actuatorHandles[ActuatorType::STOP_BEACON]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::stopBeacon);
    actuatorHandles[ActuatorType::LEAVE_CARPET]=static_cast<ActuatorCommandHandle>(&ExecutorCommon::leaveCarpet);


    suscribe();
    mapping();

}


void ExecutorCommon::stop(){
    shouldStop = true;
}

void ExecutorCommon::processActuatorCommand(Command *_command){
    commandQueueLock.lock();
    commandsToProcess.push(_command);
    commandQueueLock.unlock();
}

ActuatorCommand* ExecutorCommon::getNextCommand(){  // if there is more then one command = send error to old one onda execute new one , returns new command
    ActuatorCommand* newCommand = NULL;
    commandQueueLock.lock();

    //while (instructionQueue.empty()) {
    //    queueNotEmpty.wait(lock);
    //}

    if(!commandsToProcess.empty()){
        while(commandsToProcess.size() > 1){
            Command* cmd = commandsToProcess.front();
            commandsToProcess.pop();
            debug("newer Command, seding error to old");
            sendResponseFromCommand(cmd, ERROR);
        }
        newCommand =(ActuatorCommand*)commandsToProcess.front();
        commandsToProcess.pop();
    }

    commandQueueLock.unlock();
    return newCommand;
}

void ExecutorCommon::main(){
    shouldStop = false;
    while(!shouldStop){
        ActuatorCommand* newCommand = getNextCommand();
        if (newCommand!= NULL && currentActuatorCommand!= NULL){
            //send error to old command
            debug("Newer command recived seding error to old one");
            sendResponseFromCommand(currentActuatorCommand, ERROR);
        }

        if (newCommand!=NULL){
            debug("executing new command");
            (this->*actuatorHandles[newCommand->getActuatorType()])(newCommand); // do new command ( map static cast )

        }
        //brodcastNotification();
        liftLoop();
        boost::this_thread::sleep(boost::posix_time::milliseconds(2));
    }
}

void ExecutorCommon::mapping(){
    debug("TODO: must redefine mapping");
}

void ExecutorCommon::suscribe(){
    debug("TODO: must redefine suscribe");
}

void ExecutorCommon::brodcastNotification(){
    //debug("TODO: must redefine brodcastNotification");
}

void ExecutorCommon::reloadConfig(ActuatorCommand* _command){
    debug("reload");
    bool success = false ;
    ReloadConfig* command = (ReloadConfig*) _command;
    currentActuatorCommand = command;
    debug("reload config with exec name: ");
    success = reload(&value, executorName);
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }

}

void ExecutorCommon::getPopcorn(ActuatorCommand* _command){
    bool success = false ;
    GetPopcorn* command = (GetPopcorn*) _command;
    currentActuatorCommand = command ;
    success = GetPopcornFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::unloadPopcorn(ActuatorCommand* _command){
    bool success = false ;
    UnloadPopcorn* command = (UnloadPopcorn*) _command;
    currentActuatorCommand = command ;
    success = UnloadPopcornFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::kickRight(ActuatorCommand* _command){
    bool success = false;
    KickRight* command = (KickRight*) _command;
    currentActuatorCommand = command;
    success = KickRightFunction();
    if (success){
        debug("sendin success");
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        debug("sending error");
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::kickLeft(ActuatorCommand* _command){
    bool success = false;
    KickLeft* command = (KickLeft*) _command;
    currentActuatorCommand = command;
    success = KickLeftFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::unKickRight(ActuatorCommand* _command){
    bool success = false;
    UnKickRight* command = (UnKickRight*) _command;
    currentActuatorCommand = command;
    success = UnKickRightFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::unKickLeft(ActuatorCommand* _command){
    bool success = false;
    UnKickLeft* command = (UnKickLeft*) _command;
    currentActuatorCommand = command;
    success = UnKickLeftFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}


void ExecutorCommon::getObject(ActuatorCommand * _command){
    bool success;
    GetObject* command = (GetObject*) _command;
    currentActuatorCommand = command;
    success =  GetObjectFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::unloadObject(ActuatorCommand* _command){
    bool success;
    UnloadObject *command  = (UnloadObject*) _command;
    currentActuatorCommand = command;
    success = UnloadObjectFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::getObjectStop(ActuatorCommand * _command){
    bool success;
    GetObjectStop* command = (GetObjectStop*) _command;
    currentActuatorCommand = command;
    success =  GetObjectStopFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::startBrxon(ActuatorCommand * _command){
    bool success;
    StartBrxon* command = (StartBrxon*) _command;
    currentActuatorCommand = command;
    success =  StartBrxonFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::startBeacon(ActuatorCommand * _command){
    bool success;
    StartBeacon* command = (StartBeacon*) _command;
    currentActuatorCommand = command;
    success =  StartBeaconFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}


void ExecutorCommon::stopBrxon(ActuatorCommand * _command){
    bool success;
    StopBrxon* command = (StopBrxon*) _command;
    currentActuatorCommand = command;
    success =  StopBrxonFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}


void ExecutorCommon::stopBeacon(ActuatorCommand * _command){
    bool success;
    StopBeacon* command = (StopBeacon*) _command;
    currentActuatorCommand = command;
    success =  StopBeaconFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

void ExecutorCommon::leaveCarpet(ActuatorCommand * _command){
    bool success;
    LeaveCarpet* command = (LeaveCarpet*) _command;
    currentActuatorCommand = command;
    success =  LeaveCarpetFunction();
    if (success){
        sendResponseFromCommand(currentActuatorCommand, SUCCESS);
        currentActuatorCommand = NULL;
    }else{
        sendResponseFromCommand(currentActuatorCommand, ERROR);
        currentActuatorCommand = NULL;
    }
}

bool ExecutorCommon::KickRightFunction(){
    debug("KICK RIGHT: REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::KickLeftFunction(){
    debug("KICK LEFT: REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::UnKickRightFunction(){
    debug("UNKICK RIGHT: REDEFINE PLEASE");
     return false;
}

bool ExecutorCommon::UnKickLeftFunction(){
    debug("UNKICK LEFT: REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::GetPopcornFunction(){
    debug("REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::UnloadPopcornFunction(){
    debug("REDEFINE PLEASE");
    return false;
}
bool ExecutorCommon::UnloadObjectFunction(){
    debug("REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::GetObjectFunction(){
    debug("REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::SetSpeedFunction(){
    debug("REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::SetPositionFunction(){
    debug("REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::GetObjectStopFunction(){
    debug("REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::liftLoop(){
    return true;
}

bool ExecutorCommon::StopBrxonFunction(){
    debug("REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::StartBrxonFunction(){
    debug("REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::StartBeaconFunction(){
    debug("REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::StopBeaconFunction(){
    debug("REDEFINE PLEASE");
    return false;
}

bool ExecutorCommon::LeaveCarpetFunction(){
    debug("REDEFINE PLEASE");
    return false;
}


}
