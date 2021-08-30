#include <stdio.h>
#include <ctype.h>
#include <random>
std::random_device qdexl;
std::uniform_int_distribution<char> miss(0,1);
std::uniform_int_distribution<int> move(0,4);
std::uniform_int_distribution<int> fountains(0,7); //+2..nogo
std::uniform_int_distribution<int> hp_diff(-6,-5); //quicker
std::uniform_int_distribution<int> atk_diff(1,2); //inv calcs
std::uniform_int_distribution<int> def_diff(-2,-1);
int xp = 0;
int nxtlvl = 10;
const int xpb = 3;
const int curve = 2;
int hp_max = 5;
int hp = hp_max;
int atk = 1;
int def = 0;
int enemy_hp_max;
int enemy_hp;
int enemy_atk;
int enemy_def;
int dodge;
char getkey(void);
const char cline[] = ">";
int main(void) {
	printf("You enter the dungeon...\n");
	while(1) {
		prompt:
		printf(cline);
		switch(getkey()) {
			case 'm': {
				switch(move(qdexl)) {
					case 0: {
						printf("You see a fountain. Drink? (y/n):\n");
						printf(cline);
						if(getkey() == 'y') {
							switch(fountains(qdexl)) {
								case 0:
									hp_max = hp_max + 1;
									printf("You gain 1 max hp!\n");
									break;
								case 1:
									if(hp_max > 5) {
										hp_max = hp_max - 1;
										printf("You lose 1 max hp!\n");
									} else goto lolno;
									break;
								case 2:
									atk = atk + 1;
									printf("You gain 1 atk!\n");
									break;
								case 3:
									if(atk > 1) {
										atk = atk - 1;
										printf("You lose 1 atk!\n");
									} else goto lolno;
									break;
								case 4:
									def = def + 1;
									printf("You gain 1 def!\n");
									break;
								case 5:
									if(def > 0) {
										def = def - 1;
										printf("You lose 1 def!\n");
									} else goto lolno;
									break;
								default:
									lolno:
									printf("Nothing happens.\n");
									break;
							}
						} else {
							printf("You decide to skip the fountain.\n");
						}
						break;
					}
					case 1: {
						printf("You enter a fight with a monster!\n");
						enemy_hp_max = hp + hp_diff(qdexl);
						enemy_hp = enemy_hp_max;
						enemy_atk = def + atk_diff(qdexl);
						enemy_def = atk + def_diff(qdexl);
						if((enemy_def - atk) >= 0 && (def - enemy_atk) >= 0) {
							printf("The monster instantly dies!\n");
							goto endbatt;
						}
						while(1) {
							batts:
							printf(cline);
							dodge = 0;
							switch(getkey()) {
								case 'a': {
									int base = enemy_def - atk;
									if(miss(qdexl)) {
										printf("You miss!\n");
									} else {
										if(base >= 0) {
											printf("Your attack did nothing!\n");
										} else {
											printf("You attack the monster!\n");
											enemy_hp = enemy_hp + base;
											if(enemy_hp <= 0)
												goto endbatt;
										}
									}
									break;
								}
								case 'd':
									printf("You start dodging for a turn!\n");
									dodge = 1;
									break;
								case 's':
									printf("Current Stats:\nHP: %d/%d\nXP: %d/%d\nATK: %d\nDEF: %d\n", hp, hp_max, xp, nxtlvl, atk, def);
									goto batts;
									break;
								default:
									printf("help:\na - attack\nd - dodge\ns - view stats\n");
									goto batts;
									break;
							}
							if(miss(qdexl)) {
								printf("The monster misses you!\n");
							} else {
								if(dodge && miss(qdexl)) {
									printf("You dodge the attack expertly!\n");
								} else {
									int base = def - enemy_atk;
									if(base >= 0) {
										printf("The monster's attack did nothing!\n");
									} else {
										printf("You've been hit!\n");
										hp = hp + base;
										if(hp <= 0)
											goto gameover;
									}
								}
							}
						}
						endbatt:
						printf("You have defeated the monster!\n");
						xp = xp + xpb;
						if(xp >= nxtlvl) {
							printf("Level up!!!!\n");
							hp_max = hp_max + 5;
							printf("Max hp up by 5!!!!\n");
							if(miss(qdexl)) {
								atk = atk + 1;
								printf("Attack up by 1!!!!\n");
							}
							if(miss(qdexl)) {
								def = def + 1;
								printf("Defense up by 1!!!!\n");
							}
							nxtlvl = nxtlvl * curve;
						}
						break;
					}
					default:
						if(hp < hp_max) {
							if(miss(qdexl)) {
								printf("+1 hp.\n");
								hp++;
							}
						}
						printf("You wander around.\n");
						break;
				}
				break;
			}
			case 's':
				printf("Current Stats:\nHP: %d/%d\nXP: %d/%d\nATK: %d\nDEF: %d\n", hp, hp_max, xp, nxtlvl, atk, def);
				break;
			case 'q':
				goto gameover;
				break;
			default:
				printf("help:\nm - wander around\ns - view stats\nq - quit\n");
		}
	}
	gameover:
	printf("You die.\n");
	return 0;
}
char getkey(void) {
	char key = getchar();
	while(getchar() != '\n');
	return tolower(key);
}
