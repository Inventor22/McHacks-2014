#include <Servo.h>
const int Tx = 1;

const int DEG_PER_SEC = 30;

uint32_t tNow = 0, tLast = 0;
uint32_t updatePeriod = (1./DEG_PER_SEC) * 1000;

int joints[] =
{
    25, 10,
    32, 9,
    A6, 6,
    A7, 5,
    A8, 4,
    A9, 3
};

const int nj = sizeof(joints)/sizeof(joints[0]);

Servo joint[nj];

uint8_t angle[nj];

uint32_t timeLast = 0;

uint32_t tLast2 = 0;

void setup()
{
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);

    pinMode(Tx, INPUT_PULLUP);

    Serial1.begin(9600);
    Serial.begin(115200);

    for (int i = 0; i < nj; i++)
        joint[i].attach(joints[i]);

    tLast = millis();
    timeLast = millis();
    tLast2 = millis();
}

int i = 1;
int pos = 5;
bool kill = false;
void loop()
{
    tNow = millis();
    if (tNow - tLast > updatePeriod && !kill)
    {
        tLast = tNow;
        pos += i;

        if (pos > 175 || pos < 5)
        {
            pos -= i;
            i *= -1;
        }
        for (int i = 0; i < nj; i++)
            joint[i].write(pos);

        digitalWrite(13, !digitalRead(13));
    }

    if (tNow - tLast2 > 10000)
    {
        kill = true;
        for (int i = 0; i < nj; i++)
            joint[i].detach();
    }

    if (Serial.available() >= 2)
    {
        while (Serial1.available() > 0)
        {
            byte h0 = Serial1.read();
            if (h0 == 'G')
            {
                byte h1 = Serial1.read();
                if (h1 == 'O')
                {
                    int dataCount = 0;
                    while (dataCount < 12 && millis() - timeLast < 500)
                    {
                        if (Serial1.available() > 0)
                        {
                            uint8_t b = Serial1.read();
                            angle[dataCount++] = b;
                            Serial.write(b);
                        }
                    }
                    if (millis() - timeLast > 500)
                    {

                    }
                }
            }
        }
    }
    while (Serial.available() > 0)
    {
        byte b = Serial.read();
        Serial1.write(b);
        Serial.write((char)b);
    }
}