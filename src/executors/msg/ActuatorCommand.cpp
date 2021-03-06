#include "ActuatorCommand.h"

namespace executor {

ExecutorsMap ExecutorName={
    {LIFT_CENTER,"LiftCenterExecutor"},
    {LIFT_LEFT,"LiftLeftExecutor"},
    {LIFT_RIGHT,"LiftRightExecutor"},
    {POPCORN,"PopcornExecutor"},
    {FLAP,"FlapExecutor"},
    {ENEMY_DETECT,"EnemyDetectorExecutor"},
    {CARPET, "CarpetExecutor"}
};

string ActuatorCommand::NAME="ActuatorCommand";

Command* ActuatorAction::LiftLeft(ActuatorType _type){
    std::cout << "Creating mssg for Lift LEFT" << std::endl;
    switch (_type){
    case GET_OBJECT :    return (Command*) new GetObject(LIFT_LEFT);
    case UNLOAD_OBJECT : return (Command*) new UnloadObject(LIFT_LEFT);
    case SET_START_CONFIG : return (Command*) new SetStartConfig(LIFT_LEFT);
    case RELOAD_CONFIG : return (Command*) new ReloadConfig(LIFT_LEFT);
    case GET_OBJECT_STOP : return (Command*) new GetObjectStop(LIFT_LEFT);
    case CALLBACK_GET_LEFT: return (Command*) new CallbackGetLeft(LIFT_LEFT);
    default : return NULL;
    }
}

Command* ActuatorAction::LiftRight(ActuatorType _type){
    std::cout << "Creating mssg for Lift RIGHT" << std::endl;
    switch (_type){
    case GET_OBJECT :    return (Command*) new GetObject(LIFT_RIGHT);
    case UNLOAD_OBJECT : return (Command*) new UnloadObject(LIFT_RIGHT);
    case SET_START_CONFIG : return (Command*) new SetStartConfig(LIFT_RIGHT);
    case RELOAD_CONFIG : return (Command*) new ReloadConfig(LIFT_RIGHT);
    case GET_OBJECT_STOP : return (Command*) new GetObjectStop(LIFT_RIGHT);
    case CALLBACK_GET_RIGHT: return (Command*) new CallbackGetRight(LIFT_RIGHT);
    default : return NULL;
    }
}

Command* ActuatorAction::LiftCenter(ActuatorType _type){
    switch (_type){
    case GET_OBJECT :    return (Command*) new GetObject(LIFT_CENTER);
    case UNLOAD_OBJECT : return (Command*) new UnloadObject(LIFT_CENTER);
    case SET_START_CONFIG : return (Command*) new SetStartConfig(LIFT_CENTER);
    case GET_BALL: return (Command*) new GetBall(LIFT_CENTER);
    case LEAVE_BALL: return (Command*) new LeaveBall(LIFT_CENTER);
    case GET_GLASS: return (Command*) new GetGlass(LIFT_CENTER);
    case LEAVE_GLASS: return (Command*) new LeaveGlass(LIFT_CENTER);
    case RELOAD_CONFIG : return (Command*) new ReloadConfig(LIFT_CENTER);
    default : return NULL;
    }
}

Command* ActuatorAction::Flap(ActuatorType _type){
    switch (_type){
    case KICK_RIGHT :    return (Command*) new KickRight(FLAP);
    case KICK_LEFT :   return (Command*) new KickLeft(FLAP);
    case UNKICK_RIGHT : return (Command*) new UnKickRight(FLAP);
    case UNKICK_LEFT  :  return (Command*) new UnKickLeft(FLAP);
    case RELOAD_CONFIG : return (Command*) new ReloadConfig(FLAP);
    default : return NULL;
    }
}
Command* ActuatorAction::Popcorn(ActuatorType _type){
    switch (_type){
    case UNLOAD_POPCORN :    return (Command*) new UnloadPopcorn(POPCORN);
    case GET_POPCORN : return (Command*) new GetPopcorn(POPCORN);
    case SET_START_CONFIG : return (Command*) new SetStartConfig(POPCORN);
    case RELOAD_CONFIG : return (Command*) new ReloadConfig(POPCORN);
    default : return NULL;
    }
}

Command* ActuatorAction::EnemyDetector(ActuatorType _type){
    switch(_type){
    case START_BEACON: return (Command*) new StartBeacon(ENEMY_DETECT);
    case STOP_BEACON: return (Command*) new StopBeacon(ENEMY_DETECT);
    case START_BRXON: return ( Command*) new StartBrxon(ENEMY_DETECT);
    case STOP_BRXON: return (Command*) new StopBrxon(ENEMY_DETECT);
    case STOP_DETECTION: return (Command*) new StopDetection(ENEMY_DETECT);
    case START_DETECTION: return (Command*) new StartDetection(ENEMY_DETECT);
    default: return NULL;
    }
}


Command* ActuatorAction::Carpet(ActuatorType _type){
    switch(_type){
    case LEAVE_CARPET: return (Command*) new LeaveCarpet(CARPET);
    case CARPET_LEAVE: return (Command*) new CarpetLeave(CARPET);
    case CARPET_POSITION_1: return (Command*) new CarpetPosition1(CARPET);
    case CARPET_POSITION_2: return (Command*) new CarpetPosition2(CARPET);
    case CARPET_POSITION_OPEN: return (Command*) new CarpetPositionOpen(CARPET);
    case CARPET_POSITION_CLOSE: return (Command*) new CarpetPositionClose(CARPET);
    default: return NULL;
    }
}

Command* ActuatorConfig::LiftLeft(ActuatorType _actuator, ServoType _servo, int _value){
    switch (_actuator){
    case SET_POSITION: return (Command*) new SetPosition(LIFT_LEFT, _servo, _value);
    case SET_SPEED: return (Command*) new SetSpeed(LIFT_LEFT, _servo, _value);
    default : {
        printf("Actuator Confing does not have this ActuatorType option ");
        return NULL;
    }
    }
}

Command* ActuatorConfig::LiftRight(ActuatorType _actuator, ServoType _servo, int _value){
    switch (_actuator){
    case SET_POSITION: return (Command*) new SetPosition(LIFT_RIGHT, _servo, _value);
    case SET_SPEED: return (Command*) new SetSpeed(LIFT_RIGHT, _servo, _value);
    default : {
        printf("Actuator Confing does not have this ActuatorType option ");
        return NULL;
    }
    }
}

Command* ActuatorConfig::LiftCenter(ActuatorType _actuator, ServoType _servo, int _value){
    switch (_actuator){
    case SET_POSITION: return (Command*) new SetPosition(LIFT_CENTER, _servo, _value);
    case SET_SPEED: return (Command*) new SetSpeed(LIFT_CENTER, _servo, _value);
    default : {
        printf("Actuator Confing does not have this ActuatorType option ");
        return NULL;
    }
    }
}

Command* ActuatorConfig::Popcorn(ActuatorType _actuator, ServoType _servo, int _value){
    switch (_actuator){
    case SET_POSITION: return (Command*) new SetPosition(POPCORN, _servo, _value);
    case SET_SPEED: return (Command*) new SetSpeed(POPCORN, _servo, _value);
    default : {
        printf("Actuator Confing does not have this ActuatorType option ");
        return NULL;
    }
    }
}

Command* ActuatorConfig::Flap(ActuatorType _actuator, ServoType _servo, int _value){
    switch (_actuator){
    case SET_POSITION: return (Command*) new SetPosition(FLAP, _servo, _value);
    case SET_SPEED: return (Command*) new SetSpeed(FLAP, _servo, _value);
    default : {
        printf("Actuator Confing does not have this ActuatorType option ");
        return NULL;
    }
    }
}


string ActuatorCommandJS::NAME = "ActuatorCommand";

void ActuatorCommandJS::Init(Handle<Object> exports){
    Isolate* isolate = Isolate::GetCurrent();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, ActuatorCommandJS::NAME.c_str()));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    JavaScriptMessageProvider* provider=static_cast<JavaScriptMessageProvider*>(isolate->GetData(0));
    provider->setObjectConstructor(ActuatorCommandJS::NAME, tpl->GetFunction());
    provider->setObjectTemplate(ActuatorCommandJS::NAME, tpl->InstanceTemplate());
    exports->Set(String::NewFromUtf8(isolate, ActuatorCommandJS::NAME.c_str()), tpl->GetFunction());
}

void ActuatorCommandJS::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    bool success;

    if (args.IsConstructCall()) {
        // Invoked as constructor: `new MyObject(...)`

        v8::String::Utf8Value executorV8string ( args[0]->ToString() ) ;
        v8::String::Utf8Value actionV8string ( args[1]->ToString() );

        string _executorString = string(*executorV8string);
        string _actionString = string(*actionV8string);

        Command* obj;

        obj = parseCreateCommand(_executorString, _actionString, &success);
        //std::cout<<"returned from parseCreateCommand" << std::endl;
        if(success){
            obj->Wrap(args.This());
            args.GetReturnValue().Set(args.This());
        }else{
            isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Wrong parameters, wrong executor or aciton"));
        }
    } else {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Cannot call constructor as function."));
    }
}

Command* ActuatorCommandJS::parseCreateCommand(string _executorString, string _actionString, bool* _success){
    Executors executorTmp;
    ActuatorType actionTmp;
    Command* tmp;
    *_success = true;

    if (_executorString.compare("LiftLeft") == 0 ) executorTmp = LIFT_LEFT;
    else if(_executorString.compare("LiftRight") == 0) executorTmp = LIFT_RIGHT;
    else if(_executorString.compare("LiftCenter") == 0) executorTmp = LIFT_CENTER;
    else if(_executorString.compare("Flap") == 0) executorTmp = FLAP;
    else if(_executorString.compare("Popcorn") == 0) executorTmp = POPCORN;
    else if(_executorString.compare("EnemyDetector") == 0) executorTmp = ENEMY_DETECT;
    else if(_executorString.compare("Carpet") == 0) executorTmp = CARPET;
    else {
        executorTmp = NULL_EXECUTOR;
        *_success = false;
    }

    if (executorTmp == LIFT_LEFT || executorTmp == LIFT_RIGHT ) {
        if(_actionString.compare("StartGetting") == 0) actionTmp = GET_OBJECT;
        else if(_actionString.compare("Unload") == 0) actionTmp = UNLOAD_OBJECT;
        else if(_actionString.compare("StopGetting") == 0) actionTmp = GET_OBJECT_STOP;
        else {
            actionTmp = NULL_ACTION;
            *_success = false;
        }
    }else if(executorTmp == LIFT_CENTER){
        if(_actionString.compare("Get") == 0) actionTmp = GET_OBJECT;
        else if(_actionString.compare("Unload") == 0) actionTmp = UNLOAD_OBJECT;
        else if(_actionString.compare("GetGlass")== 0) actionTmp = GET_GLASS;
        else if(_actionString.compare("LeaveGlass")== 0) actionTmp = LEAVE_GLASS;
        else if(_actionString.compare("GetBall")==0) actionTmp = GET_BALL;
        else if(_actionString.compare("LeaveBall") == 0) actionTmp = LEAVE_BALL;
        else {
            actionTmp = NULL_ACTION;
            *_success = false;
        }
    }else if(executorTmp == POPCORN){
        if(_actionString.compare("Get") == 0) actionTmp = GET_POPCORN;
        else if(_actionString.compare("Unload") == 0) actionTmp = UNLOAD_POPCORN;
        else {
            actionTmp = NULL_ACTION;
            *_success = false;
        }
    }else if(executorTmp == FLAP){
        if(_actionString.compare("KickLeft") == 0) actionTmp = KICK_LEFT;
        else if(_actionString.compare("KickRight") == 0) actionTmp = KICK_RIGHT;
        else if(_actionString.compare("UnkickRight") == 0) actionTmp  = UNKICK_RIGHT;
        else if(_actionString.compare("UnkickLeft") == 0) actionTmp = UNKICK_LEFT;
        else{
            actionTmp = NULL_ACTION;
            *_success = false;
        }
    }else if(executorTmp == ENEMY_DETECT){
        //std::cout << "**************** ENEMY DETECTOR *******************" << std::endl;
        if(_actionString.compare("StartBrkon") == 0) actionTmp = START_BRXON;
        else if(_actionString.compare("StopBrkon") == 0) actionTmp = STOP_BRXON;
        else if(_actionString.compare("StartBeacon") == 0) actionTmp = START_BEACON;
        else if(_actionString.compare("StopBeacon") == 0) actionTmp = STOP_BEACON;
        else if(_actionString.compare("StopSensor") == 0 ) actionTmp = STOP_DETECTION;
        else if(_actionString.compare("StartSensor") == 0) actionTmp = START_DETECTION;
        else{
            actionTmp = NULL_ACTION;
            *_success = false;
        }
    }else if(executorTmp == CARPET){
        if(_actionString.compare("Leave") == 0 ) actionTmp = LEAVE_CARPET;
        else if(_actionString.compare("PositionOpen") == 0 ) actionTmp = CARPET_POSITION_OPEN;
        else if(_actionString.compare("PositionClose") == 0 ) actionTmp = CARPET_POSITION_CLOSE;
        else if(_actionString.compare("Position1") == 0 ) actionTmp = CARPET_POSITION_1;
        else if(_actionString.compare("Position2") == 0 ) actionTmp = CARPET_POSITION_2;
        else if(_actionString.compare("OpenHolder") == 0 )actionTmp = CARPET_LEAVE;
        else{
            actionTmp = NULL_ACTION;
            *_success = false;
        }
    }

    if(!*_success){
        return NULL;
    }else{
        //std::cout << "enetered creating object " << std::endl;
        switch(executorTmp){
        case LIFT_LEFT:{ tmp = ActuatorAction::LiftLeft(actionTmp); break; }
        case LIFT_RIGHT:{ tmp = ActuatorAction::LiftRight(actionTmp); break; }
        case LIFT_CENTER:{ tmp = ActuatorAction::LiftCenter(actionTmp); break; }
        case FLAP: { tmp = ActuatorAction::Flap(actionTmp); break; }
        case POPCORN: {  tmp = ActuatorAction::Popcorn(actionTmp); break; }
        case ENEMY_DETECT: { tmp  = ActuatorAction::EnemyDetector(actionTmp); break; }
        case CARPET: { tmp = ActuatorAction::Carpet(actionTmp); break; }
        case NULL_EXECUTOR: {break; }
        }
    }
    return tmp;

}

//void  parse(string _executorString, string _actuatorString, Executors *_executor, ActuatorType _action );

} // end namespace
