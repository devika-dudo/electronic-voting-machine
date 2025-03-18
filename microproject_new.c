#include <reg51.h>
#define lcdport P2

// LCD Pins
sbit rs = P3^0;
sbit rw = P3^1;
sbit en = P3^2;
sbit start_finish = P1^5;   // Start button for individual voter// Finish button for individual voter
sbit global = P1^0; 


sbit LED1 = P0^0;  // LED1 connected to P0.0
sbit LED2 = P0^1;  // LED2 connected to P0.1
sbit LED3 = P0^2;  // LED3 connected to P0.2
sbit LED4 = P0^3;  // LED4 connected to P0.3


sbit party1 = P1^1;  // Candidate 1 (BJP)
sbit party2 = P1^2;  // Candidate 2 (Congress)
sbit party3 = P1^3;  // Candidate 3 (AAP)
sbit party4 = P1^4;  // Candidate 4 (JDU)

unsigned int vote1, vote2, vote3, vote4;
char vote_no[4];
unsigned int voting_in_progress = 0;
unsigned int election_in_progress = 0;

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

void main() {
	  P0=0xFF;
    lcdport = 0x00;
    party1 = party2 = party3 = party4 = 0;
    vote1 = vote2 = vote3 = vote4 = 0;
    start_finish = global = 0;

    lcdcmd(0x01);
    lcdint();
    lcdstring("Hold Global Start");
    lcdcmd(0xc0);
    lcdstring("To Begin Election");

    // Wait for Global Start
    while (!global);

    election_in_progress = 1;
    lcdcmd(0x01);
    lcdstring("Election Started!");
		lcdcmd(0x01);
		lcdstring("Press Start");

    while (election_in_progress) {
			  
        if (start_finish == 1 && !voting_in_progress) {  // Start voter session
            voting_in_progress = 1;

            lcdcmd(0x01);  // Clear display
            lcdstring("Welcome!!");
            longdelay(200);
            lcdcmd(0x01);
            lcdstring("Press any key");
            lcdcmd(0xc0);
            lcdstring("to vote");
            longdelay(200);

            // Display candidate options
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

            count();  // Record vote
						P0=0xFF;
            lcdcmd(0x01);
            lcdcmd(0x80);
            lcdcmd(0xc0);
            lcdstring("confirm");

            while (start_finish == 1);  // Wait for officer to press Finish
            voting_in_progress = 0;
						
						lcdcmd(0x01);
						lcdstring("Press Start");
						
        }

        // Global Stop button ends election
        if (!global) {
            lcdcmd(0x01);
            lcdstring("Election Ended!");
            longdelay(500);
            check();  // Display results
            break;
        }
    }
}

// Function to count votes
void count() {
	  P0=0xFF;
    while ((party1 == 0) && (party2 == 0) && (party3 == 0) && (party4 == 0));

    if (party1 == 1) {
			   LED1=0;
        lcdcmd(0x01);
        lcdcmd(0x80);
        vote1++;
        lcdstring("VOTED: VR ");
        longdelay(300);
    }
    if (party2 == 1) {
			  LED2=0;
        lcdcmd(0x01);
        lcdcmd(0x80);
        vote2++;
        lcdstring("VOTED: SRS");
        longdelay(300);
    }
    if (party3 == 1) {
			   LED3=0;
   			lcdcmd(0x01);
        lcdcmd(0x80);
        vote3++;
        lcdstring("VOTED: VK ");
        longdelay(300);
    }
    if (party4 == 1) {
			   LED1=0;
			lcdcmd(0x01);
        lcdcmd(0x80);
        vote4++;
        lcdstring("VOTED: HPR");
        longdelay(300);
    }
}

// Function to check results
void check() {
    lcdcmd(0x01);
    lcdstring("Results:");
    longdelay(500);

    lcdcmd(0x80);
    lcdstring("VR: ");
    display_vote(vote1);
		longdelay(500);
		lcdcmd(0x01);
    lcdcmd(0x80);
    lcdstring("SRS: ");
    display_vote(vote2);
		longdelay(500);
		lcdcmd(0x01);
    lcdcmd(0x80);
    lcdstring("VK: ");
    display_vote(vote3);
    longdelay(500);
		lcdcmd(0x01);
    lcdcmd(0x80);
    lcdstring("HPR: ");
    display_vote(vote4);

    longdelay(1000);
    result();
}

// Function to display result
void result() {
    int max = 0;
    char winners[20];  // Fixed-size array for winners
    int index = 0;

    // Determine the maximum vote count
    if (vote1 > max) { max = vote1; }
    if (vote2 > max) { max = vote2; }
    if (vote3 > max) { max = vote3; }
    if (vote4 > max) { max = vote4; }

    longdelay(100);

    lcdcmd(0x01);
    lcdcmd(0x80);
    lcdstring("Winner: ");
    longdelay(250);

    // Add all candidates with max votes to winners list
    if (vote1 == max) {
        winners[index++] = 'V';  
        winners[index++] = 'R';  
        winners[index++] = ' ';  // Space separator
    }
    if (vote2 == max) {
        winners[index++] = 'S';
        winners[index++] = 'R';
        winners[index++] = 'S';
        winners[index++] = ' ';
    }
    if (vote3 == max) {
        winners[index++] = 'V';
        winners[index++] = 'K';
        winners[index++] = ' ';
    }
    if (vote4 == max) {
        winners[index++] = 'H';
        winners[index++] = 'P';
        winners[index++] = 'R';
        winners[index++] = ' ';
    }

    // Null terminate the string
    winners[index] = '\0';

    // Display the winners
    lcdcmd(0xc0);  // Move cursor to second line
    lcdstring(winners);  // Print all winners

    longdelay(500);
    lcdcmd(0x01);  // Clear LCD after displaying
}

// Function to display votes
void display_vote(unsigned int vote) {
    int k, p;
    for (k = 0; k <= 2; k++) {
        vote_no[k] = vote % 10;
        vote = vote / 10;
    }
    for (p = 2; p >= 0; p--) {
        lcddata(vote_no[p] + 48);
    }
}

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
