// -----------------------------------------
// webbot
// an internet of things robot
// -----------------------------------------


int led = D7;
int left_distanceSensor = A0;
Servo servo_back;
Servo servo_front;
int pos = 0;

int power = A5; // This is the other end of your distanceSensor. The other side is plugged into the "distanceSensor" pin (above).
// The reason we have plugged one side into an analog pin instead of to "power" is because we want a very steady voltage to be sent to the distanceSensor.
// That way, when we read the value from the other side of the distanceSensor, we can accurately calculate a voltage drop.

int left_eye; // Here we are declaring the integer variable analogvalue, which we will use later to store the value of the distanceSensor.
bool toggle;
bool blink_on;
bool toggle_time_toggler;
int toggle_time_iterations;

void setup() {
    pinMode(led,OUTPUT);
    pinMode(left_distanceSensor,INPUT);
    pinMode(power,OUTPUT); // The pin powering the distanceSensor is output (sending out consistent power)

    // Next, write the power of the distanceSensor to be the maximum possible, so that we can use this for power.
    digitalWrite(power,HIGH);

    // We are going to declare a Spark.variable() here so that we can access the value of the distanceSensor from the cloud.
    Particle.variable("left_eye", &left_eye, INT);
    // This is saying that when we ask the cloud for "left_eye", this will reference the variable left_eye in this app, which is an integer variable.

    // We are also going to declare a Spark.function so that we can turn the LED on and off from the cloud.
    Particle.function("toggle", toggle_it);

    // This is saying that when we ask the cloud for the function "toggle", it will employ the function ledToggle() from this app.
    Particle.function("front", front_set);
    Particle.function("rear", rear_set);

    toggle=false;
}

bool toggle_time(int millis, int iterations)
{
    delay(millis);
    toggle_time_iterations++;
    if(toggle_time_iterations > iterations)
    {
        toggle_time_toggler = !(toggle_time_toggler);
        toggle_time_iterations = 1;
    }
}

void servo_back_awake()
{
    servo_back.attach(D1);
}

void servo_front_awake()
{
    servo_front.attach(D2);
}

void servos_awaken()
{
    servo_back_awake();
    servo_front_awake();
}

void servos_sleep()
{
    servo_front.detach();
    servo_back.detach();
}

void led_on()
{
    digitalWrite(led,HIGH);
    blink_on = true;
}

void led_off()
{
    digitalWrite(led,LOW);
    blink_on = false;
}

void step(int rez, int pos)
{
    rez = 3;//fixed resoltion now, sliding scale mapped later
    if(pos == 1)
    {
        //first position
        //step left
        servos_awaken();
        servo_back.write(110);// +++ 110 r +++ 70 l
        servo_front.write(180);// +++ 60 right foot +++ 120 left foot

    }
    if(pos == 2)
    {
        //second position
        //neutral
        servos_awaken();
        servo_back.write(90);// +++ 110 r +++ 70 l
        servo_front.write(40);// +++ 60 right foot +++ 120 left foot
    }
    if(pos == 3)
    {
        //third position
        //step right
        servos_awaken();
        servo_back.write(70);// +++ 110 r +++ 70 l
        servo_front.write(60);// +++ 60 right foot +++ 120 left foot
    }
}


void loop() {
    // check to see what the value of the distanceSensor is and store it in the int variable analogvalue
    left_eye = analogRead(left_distanceSensor);
    int millis = 1000;
    int iterations = 3;

    if(toggle_time_iterations == iterations)
    {
        delay(millis);
        //step(iterations, 2);
        //delay(millis);
        //servos_sleep();
    }

    if(toggle==true)
    {
        if(toggle_time(millis, iterations))
        {
            //step(iterations, toggle_time_iterations);
            if(blink_on)
            {
                led_on();
            }
            else
            {
                led_off();
            }
        }
        else
        {
            //do something on 1,2 but not 3


        }
        //do something AFTER every 1000 millis


    }
    else
    {
        //servos_sleep();
        //led_off();
    }
}

int rear_set(String position) {
    servo_back_awake();
    servo_back.write(position.toInt());
    return 0;
}

int front_set(String position) {
    servo_front_awake();
    servo_front.write(position.toInt());
    return 99;
}

int toggle_it(String command) {

    if (command=="on") {
        servos_awaken();
        led_on();
        return 1;
    }
    else if (command=="off") {
        servos_sleep();
        led_off();
        return 0;
    }
    else if (command=="toggle") {
        if(toggle==true)
        {
            toggle = false;
        }
        else
        {
            toggle = true;
        }
        //var av = {
        //    "left_eye": left_eye,

        //    }
        return left_eye;
    }
    else {
        return -1;
    }
}
