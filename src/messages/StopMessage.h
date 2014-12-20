#ifndef _STOPMESSAGE_H
#define _STOPMESSAGE_H

#include "Message.h"

namespace robot {

class StopMessage:public Message{
public:
    StopMessage(const string& _sender):Message(STOP_MESSAGE,_sender){}
protected:
private:
};

}
#endif  //_STOPMESSAGE_H
