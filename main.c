#include <avr/io.h>
#include "avr.h"
#include "lcd.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define C_NORMAL 15
#define D_NORMAL 17
#define E_NORMAL 19
#define F_NORMAL 20
#define G_NORMAL 22
#define A_NORMAL 24
#define B_NORMAL 26
#define C_NORMALPLUS 27

#define WHOLE 1
#define HALF 2
#define QUARTER 4
#define EIGHTH 8
#define SIXTEENTH 16

typedef struct ChordProgression ChordProgression;
struct ChordProgression
{
  unsigned int chord1, chord2, chord3, chord4;
};

void leftRotatebyOne(int arr[], int n); 
  
void leftRotate(int arr[], int d, int n) 
{ 
    int i; 
    for (i = 0; i < d; i++) 
        leftRotatebyOne(arr, n); 
} 
  
void leftRotatebyOne(int arr[], int n) 
{ 
    int temp = arr[0], i; 
    for (i = 0; i < n - 1; i++) 
        arr[i] = arr[i + 1]; 
    arr[i] = temp; 
} 

int randomNum(int lower, int upper) 
{ 
    return (rand() % (upper - lower + 1)) + lower; 
}

int indexOfKey(char* key, char** scale)
{
    int i = 0;
    while(scale[i])
    {
        if(strcmp(scale[i], key) == 0) 
        {
            break;
        }
        i++;
    }
    return i;
}

void printArray(int arr[], int n) 
{ 
    int i; 
    for (i = 0; i < n; i++) 
        printf("%d ", arr[i]); 
} 

ChordProgression generate_sequence()
{
    ChordProgression common_sequences[] = { {1, 5, 6, 4}, {5, 6, 4, 1}, {6, 4, 1, 5}, {4, 1, 5, 6}, {1, 6, 4, 5}, {1, 6, 2, 5}};
    int r = randomNum(0, 6);
    return common_sequences[r];
    //printf("%d %d %d %d", common_sequences[r].chord1, common_sequences[r].chord2, common_sequences[r].chord3, common_sequences[r].chord4);
}

char* generate_strumming_pattern()
{
    char* strumming_patterns[7] = {"D D D D","D D DU D","D D DU DU", "D DU DU D","D-U D-U D-U D-U", "BASS DU BASS DU", "TRIPLET"};
    return strumming_patterns[randomNum(0, 6)];
}

char* build_major_scale(char* key)
{
    char* chromatic_scale[12] = {"A", "A#/Bb", "B", "C", "C#/Db", "D", "D#/Eb","E", "F", "F#/Gb", "G", "G#/Ab"};
    int arr[] = {0,1,2,3,4,5,6,7,8,9,10,11};
    leftRotate(arr, indexOfKey(key, chromatic_scale), 12);
    //char* chord1, chord2, chord3, chord4, chord5, chord6, chord7;
    char* chord1[18];
    char* chord2[18];
    char* chord3[18];
    char* chord4[18];
    char* chord5[18];
    char* chord6[18];
    char* chord7[18];
    strcpy(chord1, chromatic_scale[arr[0]]);
    strcat(chord1, " Major ");
    strcpy(chord2, chromatic_scale[arr[2]]);
    strcat(chord2, " Minor ");
    strcpy(chord3, chromatic_scale[arr[4]]);
    strcat(chord3, " Minor ");
    strcpy(chord4, chromatic_scale[arr[5]]);
    strcat(chord4, " Major ");
    strcpy(chord5, chromatic_scale[arr[7]]);
    strcat(chord5, " Major ");
    strcpy(chord6, chromatic_scale[arr[9]]);
    strcat(chord6, " Minor ");
    strcpy(chord7, chromatic_scale[arr[11]]);
    strcat(chord7, " diminished ");
    char* scale[] = {chord1, chord2, chord3, chord4, chord5, chord6, chord7};
    ChordProgression progression = generate_sequence();
    char* res[72];
    strcpy(res, scale[progression.chord1-1]);
    strcat(res, scale[progression.chord2-1]);
    strcat(res, scale[progression.chord3-1]);
    strcat(res, scale[progression.chord4-1]);
    return res;
    
}

static int is_pressed(int r, int c){
	DDRC = 0;
	PORTC = 0;
	SET_BIT(DDRC, r);
	SET_BIT(PORTC, c);
	avr_wait(1);
	if(GET_BIT(PINC, c) == 0)
	{
		return 1;
	}
	return 0;
}

static int get_key(){
	int r, c;
	for(r = 0; r < 4; ++r)
	{
		for(c = 0; c < 4; ++c){
			if(is_pressed(r,c+4)){
				return (r*4 + c) + 1;
			}
		}
	}
	return 0;
}

struct note{
	int freq;
	int duration;
};

double timestwo = 2.0;

void play_note(int freq, int duration){
	double ratio = duration/32.0;
	double converted_freq = pow(2,(freq/12.0)) * 440;//783
	converted_freq = converted_freq / timestwo;
	double period = 1.0/converted_freq;//0.00127553
	period = period / 0.00001;
	double TH = 1*(period/10.0);//255
	double TL = period - TH;
	double k = (ratio/0.00001) / period;
	//double modified_duration = duration/0.00005;
	//double k = modified_duration/period;
	int i;
	for (i = 0; i < k; ++i)
	{
		SET_BIT(PORTB, 3);
		avr_wait(TH);
		CLR_BIT(PORTB,3);
		avr_wait(TL);
	}
}

void play_music_note(int freq, int duration){
	double ratio = duration/4.0;
	double converted_freq = pow(2,(freq/12.0)) * 440;//783
	converted_freq = converted_freq / timestwo;
	double period = 1.0/converted_freq;//0.00127553
	period = period / 0.00001;
	double TH = 1*(period/10.0);//255
	double TL = period - TH;
	double k = (ratio/0.00001) / period;
	//double modified_duration = duration/0.00005;
	//double k = modified_duration/period;
	int i;
	for (i = 0; i < k; ++i)
	{
		SET_BIT(PORTB, 3);
		avr_wait(TH);
		CLR_BIT(PORTB,3);
		avr_wait(TL);
	}
}

int calc_bpm(int bpm){
	return 60000/bpm;
}

void calc_wait(int s, int note_type)
{
	int bpm = calc_bpm(s);
	switch(note_type)
	{
		case 1://x/1 time
			avr_wait_normal(bpm *4);
			break;
		case 2://x/2 time
			avr_wait_normal(bpm *2);
		case 4://x/4 time
			avr_wait_normal(bpm);
			break;
		case 8:
			avr_wait_normal(bpm /2);
			break;
		case 16:
			avr_wait_normal(bpm/4);
			break;
	}
	
}

int stressed_beat = 0;

void run_metronome(int s, int num_beats, int note_type)
{
	//int bpm = calc_bpm(s);
	char buf0[17];
	sprintf(buf0,"-----------------");
	buf0[num_beats] = '\0';
	
	for(int i = 0; i < num_beats; i++)
	{
		int interrupt = get_key();
		if(interrupt == 1)
		{
			return;
		}
		lcd_puts(buf0);
		lcd_pos(0,i);
		lcd_put('*');
		
		if(i == stressed_beat)
		{
			play_note(3+12,WHOLE);
			calc_wait(s, note_type);
			lcd_pos(0,0);
			lcd_put(i);
		}
		else
		{
			play_note(3,WHOLE);
			calc_wait(s, note_type);
			lcd_pos(0,0);
			lcd_put(i);
		}
		lcd_clr();

		//avr_wait_normal(bpm);
		/*
		if(i == 0)
		{
			lcd_pos(0,0);
			lcd_puts(buf0);
			lcd_pos(1,0);
			lcd_puts("    *---");
			play_note(C+12,QUARTER);
			avr_wait_normal(bpm);
			lcd_clr();
		}
		else
		{
			switch(i)
			{
				case 1:
					lcd_pos(0,0);
					lcd_puts(buf0);
					lcd_pos(1,0);
					lcd_puts("    -*--");
					break;
				case 2:
					lcd_pos(0,0);
					lcd_puts(buf0);
					lcd_pos(1,0);
					lcd_puts("    --*-");
					break;
				case 3:
					lcd_pos(0,0);
					lcd_puts(buf0);
					lcd_pos(1,0);
					lcd_puts("    ---*");
					break;
				
			}
			play_note(C,QUARTER);
			avr_wait_normal(bpm);
			lcd_clr();
		}
		*/
		
	}
}


int c_scale[8] = {15,17,19,20,22,24,26,27};
int main(void)
{
	SET_BIT(DDRB, 3);
	char buf[17];					
	char bot[17];

	int curr_bpm = 120;
	int edit = 0;
	int tuning = 0;
	int curr_beats = 4;
	
	int notes[5] = {1,2,4,8,16};
	int curr_notes = 2;
	CLR_BIT(DDRB, 1);
	lcd_init();
	

	while(1)
	{
		int n = get_key();
		if(!n)
		{
			run_metronome(curr_bpm,curr_beats,notes[curr_notes]);
		}
		else
		{
			switch(n){
				case 1://ENTER EDIT MODE
					edit = 1;
					while(edit)
					{
						sprintf(buf,"BPM:%d Stress:%d",curr_bpm,stressed_beat+1);
						lcd_pos(0,0);
						lcd_puts(buf);
						lcd_pos(1,0);
						sprintf(bot,"TimeSig: %d/%d",curr_beats, notes[curr_notes]);
						lcd_puts(bot);
						int k = get_key();
						switch(k)
						{
							case 3:
								if(curr_bpm < 40)
								{
									curr_bpm = 40;
								}
								else
								{
									curr_bpm--;
								}
								break;
							case 5://beats per measure
								if(curr_beats > 15)
								{
									curr_beats = 16;
								}
								else
								{
									curr_beats++;
								}
								break;
							case 6://beats per measure
								if(curr_beats < 2)
								{
									curr_beats = 1;
								}
								else
								{
									curr_beats--;
								}
								break;
							case 13://type of notes per measure
								if(curr_notes < 1)
								{
									curr_notes = 0;
								}
								else
								{
									curr_notes--;
								}
								break;
							case 14://type of notes per measure
								if(curr_notes > 3)
								{
									curr_notes = 4;
								}
								else
								{
									curr_notes++;
								}
								break;
							case 4:
								if(curr_bpm > 239)
								{
									curr_bpm = 240;
								}
								else
								{
									curr_bpm++;
								}
								break;
							case 7:
								stressed_beat++;
								stressed_beat %= curr_beats;
								break;
							case 8:
								if (stressed_beat < 1)
								{
									stressed_beat = 0;
								}
								else
								{
									stressed_beat--;
								}	
								break;
							case 16:
								edit = 0;
								break;
						}
						avr_wait_normal(200);
						lcd_clr();
					}
					case 2:
						progression = 1;
						lcd_pos(0,0);
						while(progression)
						{
							int progession_key = get_key();
							switch(progession_key){
								case 5:
									lcd_puts(build_major_scale("C"));
									break;
								case 6:
									lcd_puts(build_major_scale("D"));
									break;
								case 7:
									lcd_puts(build_major_scale("E"));
									break;
								case 8:
									lcd_puts(build_major_scale("F"));
									break;
								case 9:
									lcd_puts(build_major_scale("G"));
									break;
								case 10:
									lcd_puts(build_major_scale("A"));
									break;
								case 11:
									lcd_puts(build_major_scale("B"));
									break;
									
								case 16:
									progession = 0;
									break;
							}
							avr_wait_normal(200);
							lcd_clr();
						}
						break;
					/*
					case 2://PIANO MODE
						tuning = 1;
						lcd_pos(0,0);
						while(tuning)
						{
							int string = get_key();
							switch(string){
								case 5:
									play_note(c_scale[0], 4);
									lcd_put('C');
									break;
								case 6:
									play_note(c_scale[1], 4);
									lcd_put('D');
									break;
								case 7:
									play_note(c_scale[2], 4);
									lcd_put('E');
									break;
								case 8:
									play_note(c_scale[3], 4);
									lcd_put('F');
									break;
								case 9:
									play_note(c_scale[4], 4);
									lcd_put('G');
									break;
								case 10:
									play_note(c_scale[5], 4);
									lcd_put('A');
									break;
								case 11:
									play_note(c_scale[6], 4);
									lcd_put('B');
									break;
								case 12:
									play_note(c_scale[7], 4);
									lcd_put('C');
									break;
								case 13:
									if(c_scale[0] == 3)
									{
										
									}
									else
									{
										for(int i = 0; i < 8; i++)
										{
											c_scale[i] -= 12;
										}
										lcd_puts("-1 Octave");
									}
									break;
								case 14:
										if(c_scale[0] == 27)
										{
											
										}
										else
										{
											for(int i = 0; i < 8; i++)
											{
												c_scale[i] += 12;
											}
											lcd_puts("+1 Octave");
										}
									break;
									
								case 16:
									tuning = 0;
									break;
							}
							avr_wait_normal(200);
							lcd_clr();
						}
						break;
					*/

			}
			
		}
		

		
	}
	return 0;
}

