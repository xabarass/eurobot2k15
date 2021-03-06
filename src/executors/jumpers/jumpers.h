
#ifndef _JUMPERS
#define _JUMPERS

//#define CROSS_COMPILING
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp> // za deley

#ifdef CROSS_COMPILING

#include "executors/AbstractExecutor.h"

#include <wiringPi.h>
#include "messages/StartMatchMessage.h"
#include "messages/Notification.h"
#include "executors/msg/SideNotification.h"

using namespace robot;
#endif

namespace executor{
#ifdef CROSS_COMPILING
class JumpersExecutor : public AbstractExecutor {
public:
    static string NAME;
    JumpersExecutor():AbstractExecutor(this->NAME){}
private:
    void stop();
    void main();
    void init();

    bool shouldStop = false;

    const int start_jumper = 25;
    const int side_jumper = 24;
    const int led_workiing_RED = 22;
    const int led_side_YELLOW = 27;
    const int led_side_GREEN = 23;

    StartMessage::Color color;

};
#endif

} // end namespace

#endif
