int temt_vcc = 14;
int temt_gnd = 16;
int temt_sig = A10;

// basic low-pass filter
#define FILDEPTH 32
// Propagate value in roughly 3s
#define CYCLE_MS 100
// low-light values are too bad, expand range to make average less affected by rounding errors
#define ZOOM_FAC 4
// interval count after which to report forcibly, even when smooth value not changed 
#define KEEP_ALIVE_PUSH 32

unsigned fildata[FILDEPTH];
unsigned fil_sum = 0;
unsigned fil_idx = 0;
unsigned old_result = 0;
unsigned cycle_count = 0;

unsigned get_cur() {
  return ((unsigned) ZOOM_FAC) * ((unsigned) analogRead(temt_sig));
}

void make_board_dark(unsigned beDark) {
  return;
  // FIXME: not working for unknown reason, check https://arduino.stackexchange.com/questions/3434/leonardo-disable-tx-and-rx-led again
  
  if (beDark) {
    //pinMode(led, INPUT);
    //digitalWrite(led, LOW);
    //pinMode(led_pwr, INPUT);
    //digitalWrite(led_pwr, LOW);
    TXLED0;
    RXLED0;
  }
  else {
    //digitalWrite(led, HIGH);
    TXLED1;
    RXLED1;
  }
}

unsigned get_serial() {
  unsigned silence = 0, ret = Serial.available();
  for (unsigned n = ret; n > 0; --n)
  {
    int c = Serial.read();
    if (c < 0)
      break;
//    if (c == 's')
//      make_board_dark(1);
  }
  return ret;
}

// the setup routine runs once when you press reset:
void setup() {
  // setup power for the TEMT sensor
  pinMode(temt_vcc, OUTPUT);
  pinMode(temt_gnd, OUTPUT);
  digitalWrite(temt_vcc, HIGH);
  digitalWrite(temt_gnd, LOW);
  // setup communication, make sure to be ready
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  // setup initial LPF values
  old_result = get_cur();
  for(int i = 0; i < FILDEPTH; ++i)
    fildata[i]=old_result;
  fil_sum = old_result * FILDEPTH;
  
  // make_board_dark(1);
}

// the loop routine runs over and over again forever:
void loop() {
  unsigned result, cur_val, force_print;
  
  cur_val = get_cur();
  
  fil_idx = (++fil_idx) % FILDEPTH;
  fil_sum -= fildata[fil_idx];
  fildata[fil_idx] = cur_val;
  fil_sum += cur_val;
  result = fil_sum / FILDEPTH;

  // check and discard any input, just care about the ping!
  force_print = get_serial();

  // no useless dupes unless requested
  if(old_result != result || 0 != force_print || cycle_count > KEEP_ALIVE_PUSH) {  
    old_result = result;
  
    static char fmt[40], fmt2[40];
    //sprintf(fmt, "SmoothSnapshotRingsumInbuf: %u - %u - %u [%d]", result, cur_val, fil_sum, force_print);
    sprintf(fmt, "Smooth: %u;", result);
    Serial.println(fmt);
    sprintf(fmt2, "Raw: %u;", cur_val);
    Serial.println(fmt2);
    Serial.flush();
  }
  delay(CYCLE_MS);
  cycle_count = cycle_count > KEEP_ALIVE_PUSH ? 0 : cycle_count + 1;
}
