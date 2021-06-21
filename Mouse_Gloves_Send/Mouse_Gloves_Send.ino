// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// ----- ----- -----   기울기센서 오픈 소스  ----- ----- -----
// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
void displaySensorDetails(void) {
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}
void displayDataRate(void) {
  Serial.print  ("Data Rate:    ");
  switch (accel.getDataRate()) {
    case ADXL345_DATARATE_3200_HZ:
      Serial.print  ("3200 ");
      break;
    case ADXL345_DATARATE_1600_HZ:
      Serial.print  ("1600 ");
      break;
    case ADXL345_DATARATE_800_HZ:
      Serial.print  ("800 ");
      break;
    case ADXL345_DATARATE_400_HZ:
      Serial.print  ("400 ");
      break;
    case ADXL345_DATARATE_200_HZ:
      Serial.print  ("200 ");
      break;
    case ADXL345_DATARATE_100_HZ:
      Serial.print  ("100 ");
      break;
    case ADXL345_DATARATE_50_HZ:
      Serial.print  ("50 ");
      break;
    case ADXL345_DATARATE_25_HZ:
      Serial.print  ("25 ");
      break;
    case ADXL345_DATARATE_12_5_HZ:
      Serial.print  ("12.5 ");
      break;
    case ADXL345_DATARATE_6_25HZ:
      Serial.print  ("6.25 ");
      break;
    case ADXL345_DATARATE_3_13_HZ:
      Serial.print  ("3.13 ");
      break;
    case ADXL345_DATARATE_1_56_HZ:
      Serial.print  ("1.56 ");
      break;
    case ADXL345_DATARATE_0_78_HZ:
      Serial.print  ("0.78 ");
      break;
    case ADXL345_DATARATE_0_39_HZ:
      Serial.print  ("0.39 ");
      break;
    case ADXL345_DATARATE_0_20_HZ:
      Serial.print  ("0.20 ");
      break;
    case ADXL345_DATARATE_0_10_HZ:
      Serial.print  ("0.10 ");
      break;
    default:
      Serial.print  ("???? ");
      break;
  }
  Serial.println(" Hz");
}
void displayRange(void) {
  Serial.print  ("Range:         +/- ");
  switch (accel.getRange()) {
    case ADXL345_RANGE_16_G:
      Serial.print  ("16 ");
      break;
    case ADXL345_RANGE_8_G:
      Serial.print  ("8 ");
      break;
    case ADXL345_RANGE_4_G:
      Serial.print  ("4 ");
      break;
    case ADXL345_RANGE_2_G:
      Serial.print  ("2 ");
      break;
    default:
      Serial.print  ("?? ");
      break;
  }
  Serial.println(" g");
}
// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// ----- ----- -----   기울기센서 오픈 소스  ----- ----- -----
// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

//기울기 센서값을 저장할 int형 x, y 변수
int x;
int y;

int led = 4;//LED 아두이노 4번핀 사용
int led_state = 0;//장갑마우스 기능 ON OFF 상태 변수 [ 0 = on , 1 = off ]

int button_id = A2;//장갑마우스 ON OFF 설정 압력센서 아두이노 A2번핀 사용
int button_value = 0;
int button_state = 0;
int button_count_1;
int button_count_2;

#include <SoftwareSerial.h>
#define RxD 2
#define TxD 3
SoftwareSerial blueToothSerial(RxD, TxD);//블루투스 시리얼핀 아두이노 2번,3번 핀 사용

long lastMillis = 0;
long currentMillis = 0;

void setup() {

  // 기울기센서 오픈 소스
  if (!accel.begin()) {
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while (1);
  }
  accel.setRange(ADXL345_RANGE_16_G);
  displaySensorDetails();
  displayDataRate();
  displayRange();
  // ----- ----- ----- ----- ----- ----- ----- ----- ----- -----


  blueToothSerial.begin(9600);
  Serial.begin(115200);

  pinMode(led, OUTPUT);//LED pinMode 설정 [OUTPUT]

  pinMode(button_id, INPUT_PULLUP);//압력센서들 pinMode 설정 [INPUT_PULLUP]
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);

}



void loop() {

  if (led_state == 0) {//장갑마우스 기능 ON 상태일경우

    currentMillis = millis();//currentMillis에 현재 시간 저장
    if (currentMillis - lastMillis >= 50) {//currentMillis - lastMillis 값이 50보다 크면 [ 0.05초 마다 실행됨 ]
      lastMillis = currentMillis;//lastMillis 변수에 currentMillis 값 저장

      //현재 기울기값 호출
      get_sensor();

      //map 함수로 기울기의 정도를 1~9로 변환
      x = map(x, 60, -60, 1, 9);
      y = map(y, 60, -60, 1, 9);

      //변환한 x값이 9보다 크면 9로 고정
      if (x > 9) {
        x = 9;
      }
      if (y > 9) {
        y = 9;
      }
      //변환한 x값이 1보다 작으면 1로 고정
      if (x < 1) {
        x = 1;
      }
      if (y < 1) {
        y = 1;
      }

      int l, r;

      //A0에 연결된 압력센서가 감지되면 [ 마우스 좌클릭 ]
      if (digitalRead(A0) == 0) {
        l = 1;// l변수를 1로 변경
      } else {//A0 연결된 압력센서가 감지안되면 [ 마우스 좌클릭 해제 ]
        l = 0;// l변수를 0으로 변경
      }

      if (digitalRead(A1) == 0) {// [ 마우스 우클릭 ]
        r = 1;
      } else {
        r = 0;
      }

      // 레오나르도 보드로 전송할 데이터를 생성한다
      //@ + l + r + x + y
      //@ + 왼쪽클릭 + 오른쪽클릭 + 기울기x + 기울기y
      
      String send_date = "@";
      send_date = send_date + l;
      send_date = send_date + r;
      send_date = send_date + x;
      send_date = send_date + y;

      blueToothSerial.print(send_date);

    }
  }


  button_value = digitalRead(button_id);//세번째 압력센서 값을 받아온다
  if (button_state == 0 && button_value == 0) {//button_state = 0 이고, 압력이 감지되면
    button_count_1++;//button_count_1 값 1증가
    button_count_2 = 0;//button_count_2 값 초기화
    if (button_count_1 >= 50) {//압력 감지가 50번 이상 연속으로 감지되면 (정확한 동작을 위해서)
      button_state = 1;//button_state 변수값 1로 변경
    }
  }
  if (button_state == 1 && button_value == 1) {//button_state = 1이고, 압력감지가 없으면
    button_count_2++;//button_count_2 값 1 증가
    button_count_1 = 0;//button_count_1 값 초기화
    if (button_count_2 >= 50) {//압력감지가 없음을 연속 50번 이상 감지되면 (정확한 동작을 위해서)
      button_state = 0;//button_state 변수값 초기화

      if (led_state == 0) {//장갑마우스 기능이 ON 이라면
        led_state = 1;//장갑마우스 기능을 OFF 로 변경하고
        digitalWrite(led, HIGH);//LED ON
      } else {//그 반대
        led_state = 0;//
        digitalWrite(led, LOW);
      }

    }
  }

  delay(1);
}



//기울기 x, y 데이터 호출 함수
void get_sensor() {
  sensors_event_t event;
  accel.getEvent(&event);

  float get_x, get_y;

  get_x = event.acceleration.x;
  get_y = event.acceleration.y;

  x = get_x * 10;
  y = get_y * 10;
}


