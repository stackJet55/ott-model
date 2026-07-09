#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TV_SHOWS 100
#define MAX_MOVIES 100
#define MAX_ACTORS 500
#define MAX_USERS 100
#define NAME_LEN 50
#define DAYS_IN_WEEK 7
#define MAX_DAYS_LEN 10
#define MAX_LANGUAGES 8
#define MAX_LANGUAGE_NAME_LEN 15
#define MAX_AWARDS 20
#define MAX_HISTORY 50

int show_count = 0, movie_count = 0, cast_total = 0, user_count = 0;

struct date {
    short day;
    short month;
    short year;
};

struct Awards {
    char MediaName[NAME_LEN];
    char Category[NAME_LEN];
    char Type[NAME_LEN];
    short int Year;
    char Result[10];
    char Notes[100];
};

struct PersonalDetails {
    char Name[NAME_LEN];
    struct date DOB;
    char Languages[MAX_LANGUAGES][MAX_LANGUAGE_NAME_LEN];
    char Famous_for[NAME_LEN];
    char Movies[MAX_MOVIES][NAME_LEN];
    char TV_Shows[MAX_TV_SHOWS][NAME_LEN];
    struct Awards awards_info[MAX_AWARDS];
    int movie_count;
    int show_count;
    int award_count;
} cast_db[MAX_ACTORS];

struct Caste {
    char Actor[NAME_LEN];
    char Actress[NAME_LEN];
    char Singer[NAME_LEN];
    char Supporting[5][NAME_LEN];
};

struct TV_show {
    char Name[NAME_LEN];
    struct date original_release;
    unsigned int CurrentEp;
    struct date telecast_date;
    char Telecast_days[DAYS_IN_WEEK][MAX_DAYS_LEN];
    char Producer[NAME_LEN];
    char Writer[NAME_LEN];
    struct Caste caste_info;
    char ProductionCompany[NAME_LEN];
} show_db[MAX_TV_SHOWS];

struct Movie {
    char Name[NAME_LEN];
    struct date release_date;
    char Producer[NAME_LEN];
    char Writer[NAME_LEN];
    struct Caste caste_info;
    char Soundtrack[NAME_LEN];
    char ProductionCompany[NAME_LEN];
    float BoxOfficeCollection;
    float Budget;
    struct Awards awards[MAX_AWARDS];
    int award_count;
} movie_db[MAX_MOVIES];

struct ViewingDetails {
    char MediaName[NAME_LEN];
    float time_spent;
    char Language[MAX_LANGUAGE_NAME_LEN];
    struct date view_date;
    float rating;
};

struct Subscription {
    char PlanName[20];
    struct date trans_date;
    float amount;
    struct date expiry_date;
};

struct User {
    unsigned long MobileNumber;
    char Name[NAME_LEN];
    struct date reg_date;
    char PlanName[20];
    char preferences[5][NAME_LEN];
    struct Subscription sub_history[10];
    struct ViewingDetails history[MAX_HISTORY];
    int history_count;
    int pref_count;
    int sub_count;
} user_db[MAX_USERS];

struct MediaYear {
    char title[NAME_LEN];
    int year;
};

struct ShowTime {
    char name[NAME_LEN];
    float time;
};

void addTVShow();
void addMovie();
void addUser();
void addNewCast();
void updateCast(char *name, char *media, int isTVShow);
void searchCastDescending();
void searchByCompanyAndCast();
void searchMovieAwards();
void sameCastingDetails();
void checkSubscriptions();
void categorizeUsers();
void sortCastAwardsDesc();
void sortShowsByViewingTime();
void maxViewedMovieByMonth();
long dateToDays(struct date d);
void sortTVShows();
void sortMovies();
void sortUsers();
void sortCastDB();

int cmp_cast(const void *a, const void *b) {
    return strcmp(((struct PersonalDetails *)a)->Name, ((struct PersonalDetails *)b)->Name);
}

int cmp_movie(const void *a, const void *b) {
    return strcmp(((struct Movie *)a)->Name, ((struct Movie *)b)->Name);
}

int cmp_tvshow(const void *a, const void *b) {
    return strcmp(((struct TV_show *)a)->Name, ((struct TV_show *)b)->Name);
}

int cmp_user(const void *a, const void *b) {
    unsigned long num1 = ((struct User *)a)->MobileNumber;
    unsigned long num2 = ((struct User *)b)->MobileNumber;
    return (num1 > num2) - (num1 < num2);
}

int cmp_media_year(const void *a, const void *b) {
    return ((struct MediaYear *)b)->year - ((struct MediaYear *)a)->year;
}

int cmp_award_year(const void *a, const void *b) {
    return ((struct Awards *)b)->Year - ((struct Awards *)a)->Year;
}

int cmp_show_time(const void *a, const void *b) {
    float diff = ((struct ShowTime *)b)->time - ((struct ShowTime *)a)->time;
    return (diff > 0) - (diff < 0);
}

long dateToDays(struct date d) {
    return d.year * 365 + d.month * 30 + d.day; 
}

void sortTVShows() {
    qsort(show_db, show_count, sizeof(struct TV_show), cmp_tvshow);
}

void sortMovies() {
    qsort(movie_db, movie_count, sizeof(struct Movie), cmp_movie);
}

void sortUsers() {
    qsort(user_db, user_count, sizeof(struct User), cmp_user);
}

void sortCastDB() {
    qsort(cast_db, cast_total, sizeof(struct PersonalDetails), cmp_cast);
}

void updateCast(char *name, char *media, int isTVShow) {
    if (strcmp(name, "0") == 0 || strlen(name) == 0) return;

    struct PersonalDetails key;
    strcpy(key.Name, name);
    
    struct PersonalDetails *found = bsearch(&key, cast_db, cast_total, sizeof(struct PersonalDetails), cmp_cast);

    if (found) {
        if (isTVShow == 1) {
            strcpy(found->TV_Shows[found->show_count++], media);
        } else {
            strcpy(found->Movies[found->movie_count++], media);
        }
    } else if (cast_total < MAX_ACTORS) {
        strcpy(cast_db[cast_total].Name, name);
        if (isTVShow == 1) {
            strcpy(cast_db[cast_total].TV_Shows[0], media);
            cast_db[cast_total].show_count = 1;
            cast_db[cast_total].movie_count = 0;
        } else {
            strcpy(cast_db[cast_total].Movies[0], media);
            cast_db[cast_total].movie_count = 1;
            cast_db[cast_total].show_count = 0;
        }
        cast_db[cast_total].award_count = 0;
        cast_total++;
        sortCastDB();
    }
}

void addTVShow() {
    struct TV_show s;
    printf("Show Name: ");
    scanf(" %[^\n]", s.Name);
    printf("Telecast Date (DD MM YYYY): ");
    scanf("%hd %hd %hd", &s.telecast_date.day, &s.telecast_date.month, &s.telecast_date.year);
    printf("Current number of Episodes: ");
    scanf("%u", &s.CurrentEp);
    printf("Days (0 to stop): ");
    int days_flag = 0;
    for (int i = 0; i < DAYS_IN_WEEK && !days_flag; i++) {
        scanf("%s", s.Telecast_days[i]);
        if (strcmp(s.Telecast_days[i], "0") == 0) days_flag = 1;
    }
    printf("Producer: ");
    scanf(" %[^\n]", s.Producer);
    printf("Writer: ");
    scanf(" %[^\n]", s.Writer);
    printf("Actor (0 to skip): ");
    scanf(" %[^\n]", s.caste_info.Actor);
    printf("Actress (0 to skip): ");
    scanf(" %[^\n]", s.caste_info.Actress);
    printf("Singer (0 to skip): ");
    scanf(" %[^\n]", s.caste_info.Singer);
    printf("Supporting Cast (enter 0 to stop):\n");
    int support_flag = 0;
    for (int i = 0; i < 5 && !support_flag; i++) {
        scanf(" %[^\n]", s.caste_info.Supporting[i]);
        if (strcmp(s.caste_info.Supporting[i], "0") == 0) support_flag = 1;
    }
    printf("Production Company: ");
    scanf(" %[^\n]", s.ProductionCompany);

    updateCast(s.caste_info.Actor, s.Name, 1);
    updateCast(s.caste_info.Actress, s.Name, 1);
    updateCast(s.caste_info.Singer, s.Name, 1);
    support_flag = 0;
    for (int i = 0; i < 5 && !support_flag; i++) {
        if (strcmp(s.caste_info.Supporting[i], "0") != 0)
            updateCast(s.caste_info.Supporting[i], s.Name, 1);
        else
            support_flag = 1;
    }

    show_db[show_count++] = s;
    sortTVShows();
}

void addMovie() {
    struct Movie m;
    printf("Movie Name: ");
    scanf(" %[^\n]", m.Name);
    printf("Release Date (dd mm yyyy): ");
    scanf("%hd %hd %hd", &m.release_date.day, &m.release_date.month, &m.release_date.year);
    printf("Producer: ");
    scanf(" %[^\n]", m.Producer);
    printf("Writer: ");
    scanf(" %[^\n]", m.Writer);
    printf("Actor (0 to skip): ");
    scanf(" %[^\n]", m.caste_info.Actor);
    printf("Actress (0 to skip): ");
    scanf(" %[^\n]", m.caste_info.Actress);
    printf("Singer (0 to skip): ");
    scanf(" %[^\n]", m.caste_info.Singer);
    printf("Supporting Cast (enter 0 to stop):\n");
    int support_flag = 0;
    for (int i = 0; i < 5 && !support_flag; i++) {
        scanf(" %[^\n]", m.caste_info.Supporting[i]);
        if (strcmp(m.caste_info.Supporting[i], "0") == 0) support_flag = 1;
    }
    printf("Soundtrack: ");
    scanf(" %[^\n]", m.Soundtrack);
    printf("Production Company: ");
    scanf(" %[^\n]", m.ProductionCompany);
    printf("Box Office Collection: ");
    scanf("%f", &m.BoxOfficeCollection);
    printf("Budget: ");
    scanf("%f", &m.Budget);
    printf("Number of awards: ");
    scanf("%d", &m.award_count);
    for (int i = 0; i < m.award_count && i < MAX_AWARDS; i++) {
        printf("Details for Award %d:\n", i + 1);
        strcpy(m.awards[i].MediaName, m.Name);
        printf("Award catagory: ");
        scanf(" %[^\n]", m.awards[i].Category);
        printf("Award type: ");
        scanf(" %[^\n]", m.awards[i].Type);
        printf("Year: ");
        scanf("%hd", &m.awards[i].Year);
        printf("Result (won/nominated): ");
        scanf(" %[^\n]", m.awards[i].Result);
        printf("Notes: ");
        scanf(" %[^\n]", m.awards[i].Notes);
    }

    updateCast(m.caste_info.Actor, m.Name, 0);
    updateCast(m.caste_info.Actress, m.Name, 0);
    updateCast(m.caste_info.Singer, m.Name, 0);
    support_flag = 0;
    for (int i = 0; i < 5 && !support_flag; i++) {
        if (strcmp(m.caste_info.Supporting[i], "0") != 0)
            updateCast(m.caste_info.Supporting[i], m.Name, 0);
        else
            support_flag = 1;
    }

    movie_db[movie_count++] = m;
    sortMovies();
}

void addUser() {
    struct User u;
    printf("ID: ");
    scanf("%ld", &u.MobileNumber);
    printf("Name: ");
    scanf(" %[^\n]", u.Name);
    printf("Reg Date (DD MM YYYY): ");
    scanf("%hd %hd %hd", &u.reg_date.day, &u.reg_date.month, &u.reg_date.year);
    printf("Preference: ");
    scanf(" %[^\n]", u.preferences[0]);
    u.pref_count = 1;
    u.history_count = 0;
    u.sub_count = 0;

    int match = 0;
    for (int i = 0; i < user_count && !match; i++) {
        for (int j = 0; j < user_db[i].pref_count && !match; j++) {
            if (strcmp(user_db[i].preferences[j], u.preferences[0]) == 0) {
                printf("Suggestions for you : \n");
                int pref_limit = user_db[i].pref_count;
                for (int k = 1; k < pref_limit; k++) {
                    printf(" %d. %s .\n", k, user_db[i].preferences[k]);
                }
                match = 1;
            }
        }
    }
    printf("Enter more preferences (0 to stop):\n");
    int pref_flag = 0;
    for (int i = 1; i < 5 && !pref_flag; i++) {
        scanf(" %[^\n]", u.preferences[i]);
        if (strcmp(u.preferences[i], "0") == 0) {
            pref_flag = 1;
        } else {
            u.pref_count++;
        }
    }
    user_db[user_count++] = u;
    sortUsers();
}

void addNewCast() {
    struct PersonalDetails c;
    printf("Cast Name: ");
    scanf(" %[^\n]", c.Name);
    printf("Famous For: ");
    scanf(" %[^\n]", c.Famous_for);
    printf("Number of awards: ");
    scanf("%d", &c.award_count);

    c.movie_count = 0;
    c.show_count = 0;

    for (int i = 0; i < c.award_count && i < MAX_AWARDS; i++) {
        printf("Media Name for Award %d: ", i + 1);
        scanf(" %[^\n]", c.awards_info[i].MediaName);
        printf("Category: ");
        scanf(" %[^\n]", c.awards_info[i].Category);
        printf("Year: ");
        scanf("%hd", &c.awards_info[i].Year);
        
        int verified = 0;
        struct Movie mKey;
        strcpy(mKey.Name, c.awards_info[i].MediaName);
        struct Movie *foundMovie = bsearch(&mKey, movie_db, movie_count, sizeof(struct Movie), cmp_movie);
        
        if (foundMovie) {
            for (int k = 0; k < foundMovie->award_count && !verified; k++) {
                if (strcmp(foundMovie->awards[k].Category, c.awards_info[i].Category) == 0 &&
                    foundMovie->awards[k].Year == c.awards_info[i].Year) {
                    verified = 1;
                }
            }
        }
        
        if (!verified) {
            struct TV_show sKey;
            strcpy(sKey.Name, c.awards_info[i].MediaName);
            struct TV_show *foundShow = bsearch(&sKey, show_db, show_count, sizeof(struct TV_show), cmp_tvshow);
            if (foundShow) verified = 1;
        }

        if (verified)
            printf(" -> Award Verified \n");
        else
            printf(" -> Award NOT found in DB!!! \n");
    }

    cast_db[cast_total++] = c;
    sortCastDB();
}

void searchCastDescending() {
    char name[NAME_LEN];
    printf("Enter Cast Name: ");
    scanf(" %[^\n]", name);

    struct MediaYear list[MAX_MOVIES + MAX_TV_SHOWS];
    int count = 0;

    struct PersonalDetails key;
    strcpy(key.Name, name);
    struct PersonalDetails *foundCast = bsearch(&key, cast_db, cast_total, sizeof(struct PersonalDetails), cmp_cast);

    if (foundCast) {
        for (int j = 0; j < foundCast->movie_count; j++) {
            strcpy(list[count].title, foundCast->Movies[j]);
            list[count].year = 0;
            struct Movie mKey;
            strcpy(mKey.Name, list[count].title);
            struct Movie *foundMovie = bsearch(&mKey, movie_db, movie_count, sizeof(struct Movie), cmp_movie);
            if (foundMovie) list[count].year = foundMovie->release_date.year;
            count++;
        }

        for (int j = 0; j < foundCast->show_count; j++) {
            strcpy(list[count].title, foundCast->TV_Shows[j]);
            list[count].year = 0;
            struct TV_show sKey;
            strcpy(sKey.Name, list[count].title);
            struct TV_show *foundShow = bsearch(&sKey, show_db, show_count, sizeof(struct TV_show), cmp_tvshow);
            if (foundShow) list[count].year = foundShow->telecast_date.year;
            count++;
        }
    }

    qsort(list, count, sizeof(struct MediaYear), cmp_media_year);

    printf("Media for %s (Descending Year):\n", name);
    for (int i = 0; i < count; i++) {
        printf("> %d : %s\n", list[i].year, list[i].title);
    }
}

void checkSubscriptions() {
    struct User key;
    printf("Enter User ID (Mobile Number): ");
    scanf("%ld", &key.MobileNumber);

    struct date current_date;
    printf("Enter Current Date (DD MM YY) for warning check: ");
    scanf("%hd %hd %hd", &current_date.day, &current_date.month, &current_date.year);

    long current_days = dateToDays(current_date);
    struct User *foundUser = bsearch(&key, user_db, user_count, sizeof(struct User), cmp_user);

    if (foundUser) {
        printf("\nUser Preferences/Subscribed Content:\n");
        for (int j = 0; j < foundUser->pref_count; j++) {
            printf("- %s\n", foundUser->preferences[j]);
        }

        if (foundUser->sub_count > 0) {
            struct Subscription last_sub = foundUser->sub_history[foundUser->sub_count - 1];
            long expiry_days = dateToDays(last_sub.expiry_date);
            if (expiry_days >= current_days && (expiry_days - current_days) <= 15) {
                printf("\n*** WARNING: Your subscription plan '%s' is about to end on %hd/%hd/%hd! ***\n",
                       last_sub.PlanName, last_sub.expiry_date.day, last_sub.expiry_date.month, last_sub.expiry_date.year);
            } else if (expiry_days < current_days) {
                printf("\n*** WARNING: Subscription EXPIRED! ***\n");
            }
        } else {
            printf("No active subscription found.\n");
        }
    } else {
        printf("User not found.\n");
    }
}

void searchByCompanyAndCast() {
    char castName[NAME_LEN], company[NAME_LEN];
    printf("Enter Cast Name: ");
    scanf(" %[^\n]", castName);
    printf("Enter Production Company: ");
    scanf(" %[^\n]", company);

    printf("\nMatching Movies:\n");
    for (int i = 0; i < movie_count; i++) {
        int cast_match = 0;
        if (strcmp(movie_db[i].caste_info.Actor, castName) == 0 ||
            strcmp(movie_db[i].caste_info.Actress, castName) == 0 ||
            strcmp(movie_db[i].caste_info.Singer, castName) == 0) {
            cast_match = 1;
        }
        int support_flag = 0;
        for (int j = 0; j < 5 && !support_flag; j++) {
            if (strcmp(movie_db[i].caste_info.Supporting[j], castName) == 0) {
                cast_match = 1;
                support_flag = 1;
            }
        }

        if (cast_match && strcmp(movie_db[i].ProductionCompany, company) == 0) {
            printf("- %s\n", movie_db[i].Name);
        }
    }

    printf("\nMatching TV Shows:\n");
    for (int i = 0; i < show_count; i++) {
        int cast_match = 0;
        if (strcmp(show_db[i].caste_info.Actor, castName) == 0 ||
            strcmp(show_db[i].caste_info.Actress, castName) == 0 ||
            strcmp(show_db[i].caste_info.Singer, castName) == 0) {
            cast_match = 1;
        }
        int support_flag = 0;
        for (int j = 0; j < 5 && !support_flag; j++) {
            if (strcmp(show_db[i].caste_info.Supporting[j], castName) == 0) {
                cast_match = 1;
                support_flag = 1;
            }
        }

        if (cast_match && strcmp(show_db[i].ProductionCompany, company) == 0) {
            printf("- %s\n", show_db[i].Name);
        }
    }
}

void searchMovieAwards() {
    struct Movie key;
    printf("Enter Movie Name: ");
    scanf(" %[^\n]", key.Name);

    struct Movie *foundMovie = bsearch(&key, movie_db, movie_count, sizeof(struct Movie), cmp_movie);

    if (foundMovie) {
        printf("Awards for %s:\n", foundMovie->Name);
        for (int j = 0; j < foundMovie->award_count; j++) {
            printf("- %s (%s) : %s in %hd\n",
                   foundMovie->awards[j].Category,
                   foundMovie->awards[j].Type,
                   foundMovie->awards[j].Result,
                   foundMovie->awards[j].Year);
        }
    } else {
        printf("Movie not found.\n");
    }
}

void sortCastAwardsDesc() {
    struct PersonalDetails key;
    printf("Enter Cast Name: ");
    scanf(" %[^\n]", key.Name);

    struct PersonalDetails *foundCast = bsearch(&key, cast_db, cast_total, sizeof(struct PersonalDetails), cmp_cast);

    if (foundCast) {
        qsort(foundCast->awards_info, foundCast->award_count, sizeof(struct Awards), cmp_award_year);
        
        printf("Awards for %s (Descending):\n", foundCast->Name);
        for (int j = 0; j < foundCast->award_count; j++) {
            printf("> %hd - %s (%s) for %s\n",
                   foundCast->awards_info[j].Year,
                   foundCast->awards_info[j].Category,
                   foundCast->awards_info[j].Result,
                   foundCast->awards_info[j].MediaName);
        }
    } else {
        printf("Cast member not found.\n");
    }
}

void categorizeUsers() {
    struct date curr;
    printf("Enter Current Date (DD MM YYYY) for calculation: ");
    scanf("%hd %hd %hd", &curr.day, &curr.month, &curr.year);
    long current_days = dateToDays(curr);

    for (int i = 0; i < user_count; i++) {
        long reg_days = dateToDays(user_db[i].reg_date);
        float years = (current_days - reg_days) / 365.0;
        if (years >= 1.5)
            printf("- %s , you are PLATINUM member ! \n", user_db[i].Name);
        else 
            printf("- %s , you are SILVER member ! \n", user_db[i].Name);
    }
}

void sortShowsByViewingTime() {
    struct ShowTime list[MAX_TV_SHOWS];

    for (int i = 0; i < show_count; i++) {
        strcpy(list[i].name, show_db[i].Name);
        list[i].time = 0;
    }

    for (int i = 0; i < user_count; i++) {
        for (int j = 0; j < user_db[i].history_count; j++) {
            struct TV_show sKey;
            strcpy(sKey.Name, user_db[i].history[j].MediaName);
            struct TV_show *foundShow = bsearch(&sKey, show_db, show_count, sizeof(struct TV_show), cmp_tvshow);
            
            if (foundShow) {
                int found_idx = foundShow - show_db;
                list[found_idx].time += user_db[i].history[j].time_spent;
            }
        }
    }

    qsort(list, show_count, sizeof(struct ShowTime), cmp_show_time);

    printf("\nTV Shows by Viewing Time:\n");
    for (int i = 0; i < show_count; i++) {
        printf("%d. %s - %.2f hrs\n", i + 1, list[i].name, list[i].time);
    }
}

void maxViewedMovieByMonth() {
    short m, y;
    printf("Enter Month and Year (MM YYYY): ");
    scanf("%hd %hd", &m, &y);

    struct MovieCount {
        char name[NAME_LEN];
        int views;
    } list[MAX_MOVIES];

    for (int i = 0; i < movie_count; i++) {
        strcpy(list[i].name, movie_db[i].Name);
        list[i].views = 0;
    }

    for (int i = 0; i < user_count; i++) {
        for (int j = 0; j < user_db[i].history_count; j++) {
            if (user_db[i].history[j].view_date.month == m && user_db[i].history[j].view_date.year == y) {
                struct Movie mKey;
                strcpy(mKey.Name, user_db[i].history[j].MediaName);
                struct Movie *foundMovie = bsearch(&mKey, movie_db, movie_count, sizeof(struct Movie), cmp_movie);
                
                if (foundMovie) {
                    int found_idx = foundMovie - movie_db;
                    list[found_idx].views++;
                }
            }
        }
    }

    int max_idx = 0;
    for (int i = 1; i < movie_count; i++) {
        if (list[i].views > list[max_idx].views)
            max_idx = i;
    }

    if (movie_count > 0 && list[max_idx].views > 0)
        printf("Most viewed movie in %hd/%hd is %s with %d views.\n", m, y, list[max_idx].name, list[max_idx].views);
    else
        printf("No movie views found for that month.\n");
}

void sameCastingDetails() {
    char actor[NAME_LEN], actress[NAME_LEN];
    printf("Enter Actor Name: ");
    scanf(" %[^\n]", actor);
    printf("Enter Actress Name: ");
    scanf(" %[^\n]", actress);

    printf("\nMedia matching Actor: '%s' and Actress: '%s'\n", actor, actress);

    for (int i = 0; i < movie_count; i++) {
        if (strcmp(movie_db[i].caste_info.Actor, actor) == 0 && strcmp(movie_db[i].caste_info.Actress, actress) == 0) {
            printf("- Movie: %s\n", movie_db[i].Name);
        }
    }

    for (int i = 0; i < show_count; i++) {
        if (strcmp(show_db[i].caste_info.Actor, actor) == 0 && strcmp(show_db[i].caste_info.Actress, actress) == 0) {
            printf("- TV Show: %s\n", show_db[i].Name);
        }
    }
}

void adminMenu() {
    int choice;
    int run = 1;

    while (run) {
        printf("\n ADMIN MENU =>");
        printf("\n1.  Add Show");
        printf("\n2.  Add Movie");
        printf("\n3.  Add User");
        printf("\n4.  Add New Cast & Verify");
        printf("\n5.  Sort Shows by View Time");
        printf("\n6.  Exit");
        printf("\nChoice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addTVShow(); break;
            case 2: addMovie(); break;
            case 3: addUser(); break;
            case 4: addNewCast(); break;
            case 5: sortShowsByViewingTime(); break;
            case 6: run = 0; break;
            default: printf("Invalid choice\n");
        }
    }
}

void userMenu() {
    int choice;
    int run = 1;

    while (run) {
        printf("\n USER MENU => ");
        printf("\n0.  Sign Up ");
        printf("\n1.  Search Cast Media ");
        printf("\n2.  Check Your Subscription");
        printf("\n3.  Search by Company & Cast");
        printf("\n4.  List Movie Awards");
        printf("\n5.  Awards won by Cast");
        printf("\n6.  Categories of Users");
        printf("\n7.  Max Viewed Movie by Month");
        printf("\n8.  Search by Exact Casting Details");
        printf("\n9.  Exit");
        printf("\nChoice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 0: addUser(); break;
            case 1: searchCastDescending(); break;
            case 2: checkSubscriptions(); break;
            case 3: searchByCompanyAndCast(); break;
            case 4: searchMovieAwards(); break;
            case 5: sortCastAwardsDesc(); break;
            case 6: categorizeUsers(); break;
            case 7: maxViewedMovieByMonth(); break;
            case 8: sameCastingDetails(); break;
            case 9: run = 0; break;
            default: printf("Invalid choice\n");
        }
    }
}

int main() {
    int mode;
    int run = 1;

    while (run) {
        printf("\n OTT Management System \n \n");
        printf("Admin (0) \nUser (1)\n");
        printf("Enter mode: ");
        scanf("%d", &mode);

        if (mode == 0)
            adminMenu();
        else if (mode == 1)
            userMenu();
        else
            printf("Invalid mode. Please enter 0 for Admin or 1 for User.\n");
    }

    return 0;
}
