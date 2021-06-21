#include <SoftwareSerial.h>
#define RxD 8
#define TxD 9
SoftwareSerial blueToothSerial(RxD, TxD);//블루투스 시리얼핀 아두이노 8번,9번 핀 사용
String read_data;//수신받은 데이터를 저장할 String 변수

void setup() {
  blueToothSerial.begin(9600);
  blueToothSerial.setTimeout(20);
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);

  Serial.begin(115200);

  Mouse.begin();

  Serial.print("     ");
  Serial.println("loop start");
}

void loop() {

  char array[5];
  if (blueToothSerial.available()) {
    //수신받은 데이터를 array 배열에 담는다
    byte leng = blueToothSerial.readBytes(array, 5);

    //수신받은 데이터 길이만큼 for문 실행
    for (int i = 0; i < leng; i++) {
      //배열에 저장된 데이터를 String 형식의 read_data 변수에 담는다
      read_data = read_data + array[i];
    }

    // 첫번째 데이터 = @ and 데이터 길이 = 5 인 경우는 데이터 정상 수신
    if (read_data.charAt(0) == '@' && read_data.length() == 5) {

      read_data = "";//해당 변수 초기화

      if (array[1] == '1') {//array[1]에 저장된 데이터 = 1 (마우스 좌 클릭)
        Mouse.press(MOUSE_LEFT);//마우스 왼쪽 클릭
      } else if (array[1] == '0') {//array[1]에 저장된 데이터 = 0 (마우스 좌 클릭 해제)
        Mouse.release(MOUSE_LEFT);//마우스 왼쪽 클릭 해제
      }

      if (array[2] == '1') {//array[2]에 저장된 데이터 = 2 (마우스 우 클릭)
        Mouse.press(MOUSE_RIGHT);//마우스 오른쪽 클릭
      } else if (array[2] == '0') {//...
        Mouse.release(MOUSE_RIGHT);//...
      }

      //array[3], array[4]는 기울기센서 값

      int x = array[3];// x라는 int형 변수에 array[3] 데이터 저장
      x = x - 53;// x 값에서 53을 뺀다 ( 수신받은 데이터는 char 형이므로 53을 빼줘야함 [아스키코드 이해 해야함] )

      int y = array[4];
      y = y - 53;

      //Serial.print("x:"); Serial.print(x);
      //Serial.print("__________y:"); Serial.println(y);

      if (x > 1 || x < -1) {// 수신받은 기울기 x값이 1보다 크거나 -1보다 작으면 [ 왼쪽이나 오른쪽으로 기울었을경우 ]

        x = x * 3;// x값에 3을 곱해준다 [ 기울기가 클수록 마우스 이동 폭이 크다 ]

      } else {// 수신받은 기울기 x값이 -1, 0, 1 일 경우 [ 평행이거나 조금 기울었을경우 ]
        x = 0;// x값을 0으로해서 마우스를 움직이지 않는다
      }

      if (y > 1 || y < -1) {
        y = y * 4;
      } else {
        y = 0;
      }

      Mouse.move(y, x);// 변환된 값만큼 마우스 이동

    } else {//데이터 수신 오류
      read_data = "";//해당 변수 초기화
    }

  }

}
