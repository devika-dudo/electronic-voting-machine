#include<reg51.h>
#define lcdport P2

// LCD Pins
sbit rs = P3^0;
sbit rw = P3^1;
sbit en = P3^2;
sbit start = P1^0;
sbit stop = P1^5;
sbit party1 = P1^1;    // Candidate 1 (BJP)
sbit party2 = P1^2;    // Candidate 2 (Congress)
sbit party3 = P1^3;    // Candidate 3 (AAP)
sbit party4 = P1^4;    // Candidate 4 (JDU)

unsigned int vote1, vote2, vote3, vote4;
char vote_no[4];
unsigned int voting_in_progress=0;

void lcdcmd(unsigned char);
void lcdint();
void lcddata(unsigned char);
void lcdstring(unsigned char*);
void delay(unsigned int);
void longdelay(unsigned int);
void display_vote(unsigned int);
void display_percent(unsigned int);
void count();
void result();
void check();
void check1();

void main() {
    lcdport = 0x00;
    party1 = party2 = party3 = party4 = 0;
    vote1 = vote2 = vote3 = vote4 = 0;
    start = stop = 0;
	  int election_in_progress = 0;
    
    lcdcmd(0x01);
    lcdint();
    lcdstring("Hold Start");
    lcdcmd(0xc0);
    lcdstring("To Begin Election");
    
	  if (check_long_press(start)) {
        election_in_progress = 1;
        lcdcmd(0x01);
        lcdstring("Election Started!");
        longdelay(500);
    }
    while (election_in_progress) {
        if (start == 1&& !voting_in_progress) { 
					  voting_in_progress=1;
      // Check if start button is pressed
            lcdcmd(0x01);  // Clear display
            lcdstring("Welcome!!");
            longdelay(200);
            lcdcmd(0x01);
            lcdstring("Press any key");
            lcdcmd(0xc0);
            lcdstring("to vote");
            longdelay(200);
            lcdcmd(0x01);
            lcdstring("VR ");
            delay(500);
            lcdcmd(0x84);
            lcdstring("SRS");
            delay(500);
            lcdcmd(0x88);
            lcdstring("VK ");
            delay(500);
            lcdcmd(0x8C);
            lcdstring("HPR");
       
            count();  // Start counting votes
            lcdcmd(0x01);
            lcdcmd(0x80);
            lcdstring("Thank you");
            longdelay(300);}}
            check();  // Check vote status
     voting_in_progress = 0;
     }


void check() {
    if (party1 == 0 && party2 == 0 && party3 == 0 && party4 == 0) {
   
            
                result();  // Display results
            }
        
    
}

void result() {
    int max = 0, flag = 0;
    lcdcmd(0x01);
    lcdstring("VR ");
    delay(500);
    lcdcmd(0x84);
    lcdstring("SRS");
    delay(500);
    lcdcmd(0x88);
    lcdstring("VK ");
    delay(500);
    lcdcmd(0x8C);
    lcdstring("HPR");
    lcdcmd(0xc0);
    display_vote(vote1);
    lcdcmd(0xc4);
    display_vote(vote2);
    lcdcmd(0xc8);
    display_vote(vote3);
    lcdcmd(0xcc);
    display_vote(vote4);

    // Find the max vote
    if (vote1 > max) { max = vote1; }
    if (vote2 > max) { max = vote2; }
    if (vote3 > max) { max = vote3; }
    if (vote4 > max) { max = vote4; }

    longdelay(500);

    // Display the winning party
    if ((vote1 == max) && (vote2 != max) && (vote3 != max) && (vote4 != max)) {
        flag = 1;
        lcdcmd(0x01);
        lcdcmd(0x80);
        lcdstring("VR ");
        lcdcmd(0xc5);
        lcdstring("Wins");
        longdelay(200);
        lcdcmd(0x01);
        lcdcmd(0x80);
        lcdstring("Votes: ");
        lcdcmd(0xc0);
        display_vote(vote1);
        display_percent(vote1);
        longdelay(500);
   
    }

    if ((vote2 == max) && (vote1 != max) && (vote3 != max) && (vote4 != max)) {
        flag = 1;
        lcdcmd(0x01);
        lcdcmd(0x80);
        lcdstring("SRS");
        lcdcmd(0xc5);
        lcdstring("Wins");
        longdelay(200);
        lcdcmd(0x01);
        lcdcmd(0x80);
        lcdstring("Votes: ");
        lcdcmd(0xc0);
        display_vote(vote2);
        display_percent(vote2);
        longdelay(500);
    }

    if ((vote3 == max) && (vote2 != max) && (vote1 != max) && (vote4 != max)) {
        flag = 1;
        lcdcmd(0x01);
        lcdcmd(0x80);
     
			lcdstring("VK ");
        lcdcmd(0xc5);
        lcdstring("Wins");
        longdelay(200);
        lcdcmd(0x01);
        lcdcmd(0x80);
        lcdstring("Votes: ");
        lcdcmd(0xc0);
        display_vote(vote3);
        display_percent(vote3);
        longdelay(500);
    }
if ((vote4 == max) && (vote2 != max) && (vote1 != max) && (vote3 != max)) {
        flag = 1;
        lcdcmd(0x01);
        lcdcmd(0x80);
        lcdstring("HPR");
        lcdcmd(0xc5);
        lcdstring("Wins");
        longdelay(200);
        lcdcmd(0x01);
        lcdcmd(0x80);
        lcdstring("Votes: ");
        lcdcmd(0xc0);
        display_vote(vote4);
        display_percent(vote4);
        longdelay(500);
    }

    // If there is a tie
    if (flag == 0) {
        lcdcmd(0x01);
        lcdcmd(0x80);
        lcdstring("Clash between");
        lcdcmd(0xc0);
        if (vote1 == max) { lcdstring("VR   "); }
        if (vote2 == max) { lcdstring("SRS  "); }
        if (vote3 == max) { lcdstring("VK   "); }
        if (vote4 == max) { lcdstring("HPR"); }
        longdelay(200);
        lcdcmd(0x01);
        lcdcmd(0x80);
        lcdstring("Votes: ");
        lcdcmd(0xc0);
        display_vote(max);
        display_percent(max);
        longdelay(500);
    }
  
    vote1 = vote2 = vote3 = vote4 = 0;

}

void display_vote(unsigned int vote) {
    int k, p;
    for (k = 0; k <= 2; k++) {
        vote_no[k] = vote % 10;
        vote = vote / 10;
    }
    for (p = 2; p >= 0; p--) {
        lcddata(vote_no[p] + 48);  // Display digits
    }
}

void display_percent(unsigned int vote) {
    int i;
    int per1;
    int p;
    int sum;
    char per[2];

    sum = vote1 + vote2 + vote3 + vote4;
    if (sum > 0) {
        per1 = (vote * 100) / sum;
    } else {
        per1 = 0;  // Avoid division by zero
    }

    for (i = 0; i <= 2; i++) {
        per[i] = per1 % 10;
        per1 = per1 / 10;
    }
    lcdcmd(0xc4);  // Display percentage
    for (p = 2; p >= 0; p--) {
        lcddata(per[p] + 48);
    }
    lcdcmd(0xc7);
    lcdstring("percent");
}

void count() {
    while ((party1 == 0)&& (party2==0)&&(party3==0)&&(party4==0)&&(stop==0)); 
{ 
    if (party1 == 1) 
   {
        lcdcmd(0x01);
        lcdcmd(0x80);
        vote1 = vote1 + 1;
        lcdstring("VOTED:VR ");
        longdelay(300);
    
    }
    if (party2 == 1) {
        lcdcmd(0x01);
        lcdcmd(0x80);
        vote2 = vote2 + 1;
        lcdstring("VOTED:SRS");
        longdelay(300);
    }
    if (party3 == 1) {
        lcdcmd(0x01);
        lcdcmd(0x80);
        vote3 = vote3 + 1;
        lcdstring("VOTED:VK ");
        longdelay(300);
    }
    if (party4 == 1) {
        lcdcmd(0x01);
        lcdcmd(0x80);
        vote4 = vote4 + 1;
        lcdstring("VOTED:HPR");
        longdelay(300);
    }
}}

// Function to create delays
void delay(unsigned int x) {
    unsigned int i;
    for (i = 0; i < x; i++);
}

void longdelay(unsigned int u) {
    unsigned int i, j;
    for (i = 0; i < u; i++) {
        for (j = 0; j < 1275; j++);
    }
}

// LCD initialization
void lcdint() {
    lcdcmd(0x38); delay(500);
    lcdcmd(0x01); delay(500);
    lcdcmd(0x0c); delay(500);
    lcdcmd(0x80); delay(500);
    lcdcmd(0x0e); delay(500);
}

// Send command to LCD
void lcdcmd(char value) {
    lcdport = value;
    rw = 0;
    rs = 0;
    en = 1;
    delay(500);
    en = 0;
}

// Display string on LCD
void lcdstring(char* p) {
    while (*p != '\0') {
        lcddata(*p);
        delay(2000);
        p++;
    }
}

// Send data to LCD
void lcddata(unsigned char value) {
    lcdport = value;
    rs = 1;
    rw = 0;
    en = 1;
    delay(500);
    en = 0;
}