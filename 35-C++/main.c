//
//  main.c
//  Final Project
//
//
//  Copyright (c) 2014  All rights reserved.
//
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>







typedef struct foods
{
    char foodname;
    double protein;
    double calories;
    double carbs;
    double fat;
    
    
}food;







food foodinfo[9];

void foodlibrary();

void userfatcalc();

double calculation(int serv1, int serv2, int serv3, double calfat1, double calfat2, double calfat3);

double totalcalfat(double bf, double lc, double dn);

int CaloriesBurned();


void OneDayFeedback(int eaten_fruits, int eaten_vegetables, int eaten_grains, int eaten_protein, int eaten_sweets, int eaten_dairy,
                   int alloted_fruits, int alloted_vegetables, int alloted_grains, int alloted_protein, int alloted_sweets, int alloted_dairy);










int main(void)
{
    char user[50];
    int option;
    printf("Welcome to Health-e Living: Your guide to a healthy lifestyle! \n");
    printf("Please enter your name: \n");
    scanf(" %[^\n]",user);
    
    printf("Ok %s, please continue filling out our credentials to begin cleansing your lifestyle! \n", user);
    printf(" First we need to count the number of calories you eat a day \n");
    
    printf("What do you eat for breakfast? \n");
    
    
    
    userfatcalc();
    
    float n = CaloriesBurned();
    
    printf("You will burn approximately %.2lf calories.\n", n);
    
    
    printf(" If you'd like to view our food library, please continue to the next menu. \n");
    printf(" Select a type of food: 1.)Plain yogurt 2.) Banana 3.) Boiled egg 4.) Chicken 5.) Rice 6.) Ceasar Salad 7.) Pasta 8.) Carrots 9.)Smoked Salmon 10.) Exit \n");
    scanf("%d", &option);
    
    foodlibrary(option);
    
    
 
    
    void OneDayFeedback();
    
    return 0;
}












void foodlibrary(int option)
{
    
    switch (option) {
        case 1:
    printf("Plain yogurt: Protein %lf Calories %lf Carbohydrates %lf Fats %lf\n\n",foodinfo[0].protein=23,foodinfo[0].calories=120,foodinfo[0].carbs=9,foodinfo[0].fat=0);
    
    foodinfo[0].foodname="plain_yogurt";
    foodinfo[0].protein=23;
    foodinfo[0].calories=120;
    foodinfo[0].carbs=9;
    foodinfo[0].fat=0;
            break;
        case 2:
    printf("Banana: Protein %lf Calories %lf Carbohydrates %lf Fats %lf\n\n",foodinfo[1].protein=1.7,foodinfo[1].calories= 90,foodinfo[1].carbs=19.8,foodinfo[1].fat=0.1);
    
    foodinfo[1].foodname="banana";
    foodinfo[1].protein=1.7;
    foodinfo[1].calories=90;
    foodinfo[1].carbs=19.8;
    foodinfo[1].fat=0.1;
            break;
            
        case 3:
    
    printf("Boiled Egg: Protein %lf Calories %lf Carbohydrates %lf Fats %lf\n\n",foodinfo[2].protein=8.1,foodinfo[2].calories=91,foodinfo[2].carbs=.5,foodinfo[2].fat=6.2);
    
    foodinfo[2].foodname="boiled egg";
    foodinfo[2].protein=8.1;
    foodinfo[2].calories=91;
    foodinfo[2].carbs=.5;
    foodinfo[2].fat=6.2;
            break;
            
        case 4:
    
    printf("Chicken: Protein %lf Calories %lf Carbohydrates %lf Fats %lf\n\n",foodinfo[3].protein=26,foodinfo[3].calories=223,foodinfo[3].carbs=2,foodinfo[3].fat=12);
    
    foodinfo[3].foodname="chicken";
    foodinfo[3].protein=26;
    foodinfo[3].calories=223;
    foodinfo[3].carbs=2;
    foodinfo[3].fat=12;
            
        case 5:
    
    printf("Rice: Protein %lf Calories %lf Carbohydrates %lf Fats %lf\n\n",foodinfo[4].protein=4,foodinfo[4].calories=169,foodinfo[4].carbs=37,foodinfo[4].fat=0);
    
    foodinfo[4].foodname="rice";
    foodinfo[4].protein=4;
    foodinfo[4].calories=169;
    foodinfo[4].carbs=37;
    foodinfo[4].fat=0;
            
            break;
            
        case 6:
    
    
    printf("Ceasar Salad: Protein %lf Calories %lf Carbohydrates %lf Fats %lf\n\n",foodinfo[5].protein=3,foodinfo[5].calories=260,foodinfo[5].carbs=7,foodinfo[5].fat=25);
    
    foodinfo[5].foodname="caesar salad";
    foodinfo[5].protein=3;
    foodinfo[5].calories=260;
    foodinfo[5].carbs=7;
    foodinfo[5].fat=25;
            break;
        case 7:
    
    printf("Pasta: Protein %lf Calories %lf Carbohydrates %lf Fats %lf\n\n",foodinfo[6].protein=7,foodinfo[6].calories=300,foodinfo[6].carbs=42,foodinfo[6].fat=1);
    
    foodinfo[6].foodname="pasta";
    foodinfo[6].protein=7;
    foodinfo[6].calories=300;
    foodinfo[6].carbs=42;
    foodinfo[6].fat=1;
            break;
    
        case 8:
    printf("Carrots: Protein %lf Calories %lf Carbohydrates %lf Fats %lf\n\n",foodinfo[7].protein=1,foodinfo[7].calories=35,foodinfo[7].carbs=8,foodinfo[7].fat=0);
    
    foodinfo[7].foodname="carrots";
    foodinfo[7].protein=1;
    foodinfo[7].calories=35;
    foodinfo[7].carbs=8;
    foodinfo[7].fat=0;
            
            break;
        
        case 9:
    
            
    printf("Smoked Salmon: Protein %lf Calories %lf Carbohydrates %lf Fats %lf\n\n",foodinfo[8].protein=11,foodinfo[8].calories=105,foodinfo[8].carbs=3,foodinfo[8].fat=5);
    
    foodinfo[8].foodname="smoked salmon";
    foodinfo[8].protein=11;
    foodinfo[8].calories=105;
    foodinfo[8].carbs=3;
    foodinfo[8].fat=5;
            
            break;
}

}









    double calculation(int serv1, int serv2, int serv3, double calfat1, double calfat2, double calfat3) //This is a double function that is used for calories and fat calculation of each meal
    {
        
        double mealcalfat;
        
        mealcalfat =(double)(serv1*calfat1) + (serv2*calfat2) + (serv3*calfat3);//This equation calculates the total calories and fats consumed at each meal.
        
        return mealcalfat;
    }



    double totalcalfat(double bf, double lc, double dn) //This is another double function that used for total calories and fats calculation.
    {
        double dailytotal;
        
        dailytotal = (double)(bf + lc+ dn); //This equation calculates the total calories and fats consumed for the day during all three meals.
        
        return dailytotal;
    }




int CaloriesBurned ()
{
    
    double calories;
    double age;
    double weight;
    double time;
    double max_heart;
    char gender;
    char hardness;
    double light;
    double moderate;
    double rigorous;
    
    printf("Male/Female? (M/F): \n");
    scanf(" %c" , &gender);
    printf("What is your age?: \n");
    scanf("%lf", &age);
    printf("How much do you want to spend working out(min): \n");
    scanf("%lf", &time);
    printf("what is your weight?(lb): \n");
    scanf("%lf", &weight);
    printf("how strenuously do you want to work out(light,moderate,rigorous use L, M, R)\n");
    scanf(" %c", &hardness);
    
    max_heart= 220-age; //formula for individual's max heart rate
    light= 0.5*max_heart; //light heart rate is 50% max rate
    moderate= .7*max_heart; // moderate heart rate is 70% max rate
    rigorous= .85*max_heart; // rigorous heart rate is 85% max rate
    
    
    if (gender== 'M') //if male use following equation to calculate predicted calories burned
    {
        if (hardness == 'L') // L, M, R represent heart rate dependent on user's input
        {
            calories= (age*.2017+weight*0.09036+((light*.6309))-55.0969)*(time/4.184);
        }
        else if (hardness == 'M')
        {
            calories= (((age*.2017)+(weight*0.09036)+(((moderate*.6309))-55.0969))*time)/4.184;
        }
        else if (hardness == 'R')
        {
            calories= (((age*.2017)+(weight*0.09036)+(((rigorous*.6309))-55.0969))*time)/4.184;
        }
        
    }
    
    else if (gender == 'F' ){ // if female use this equation
        
        if (hardness == 'L')
        {
            calories= (((age*.074)+(weight*0.05741)+(((light*.4472))-20.4022))*time)/4.184;
        }
        else if (hardness == 'M')
        {
            calories= (((age*.074)+(weight*0.05741)+(((moderate*.4472))-20.4022))*time)/4.184;
        }
        else if (hardness == 'R')
        {
            calories= (((age*.074)+(weight*0.05741)+(((rigorous*.4472))-20.4022))*time)/4.184;
        }
    }
    
    
    
    return calories;
}








void userfatcalc() //This main function for the calculates of the user's calories and fat.
{
    int BreakfastSelection, DinnerSelection, LunchSelection;
    
    double BreakfastCalories, LunchCalories, DinnerCalories, TotalCalories;
    double BreakfastFat, LunchFat, DinnerFat, TotalFat;
    
    double EggCalories, YogurtCalories, BananaCalories;
    double EggFat, YogurtFat, BananaFat;
    
    EggCalories= 78;
    EggFat=5;
    YogurtCalories= 149;
    YogurtFat=8;
    BananaCalories=105;
    BananaFat= 0.4;
    
    int EggServing=0;
    int YogurtServing=0;
    int BananaServing=0;
    
    
    double ChickenCalories, RiceCalories, BroccoliCalories;
    double ChickenFat, RiceFat, BroccoliFat;
    
    ChickenCalories= 306;
    ChickenFat=18;
    RiceCalories=216;
    RiceFat=1.8;
    BroccoliCalories=50;
    BroccoliFat=0.6;
    
    int ChickenServing=0;
    int RiceServing=0;
    int BroccoliServing=0;
    
    double PastaCalories, CarrotsCalories, FishCalories;
    double PastaFat, CarrotsFat, FishFat;
    
    PastaCalories= 182;
    PastaFat=0.9;
    CarrotsCalories=30;
    CarrotsFat=0.2;
    FishCalories=206;
    FishFat=12;
    
    
    int PastaServing=0;
    int CarrotsServing=0;
    int FishServing=0;
    
    printf("======Breakfast======"); //This calculates the breakfast calories and fat.
    do //This continues to ask the user to enter the items consumed for breakfast until he or she manually exits.
    {
        printf("\n\n1. 1 Egg \n");
        printf("2. 1 Serving of Plain Yogurt \n");
        printf("3. 1 Banana \n");
        printf("4. Proceed to the lunch selection\n");
        printf("\nPlease select the food item consumed at breakfast: ");
        scanf("%d",&BreakfastSelection);
        switch(BreakfastSelection)
        {
            case 1:
                printf("\nPlease enter the servings of eggs consumed: ");
                scanf("%d", &EggServing);
                break;
            case 2:
                printf("\nPlease enter the servings of yogurt consumed: ");
                scanf("%d", &YogurtServing);
                break;
            case 3:
                printf("\nPlease enter the servings of bananas consumed: ");
                scanf("%d", &BananaServing);
                break;
        }
    }while(BreakfastSelection != 4);
    
    BreakfastCalories =(double)calculation(EggServing, YogurtServing, BananaServing, EggCalories, YogurtCalories, BananaCalories); //This calculates the total calories consumed at breakfast.
    BreakfastFat =(double)calculation(EggServing, YogurtServing, BananaServing, EggFat, YogurtFat, BananaFat);  //This calculates the total fat consumed at breakfast.
    printf("\nBreakfast Calories: %.2lf \n", BreakfastCalories);
    printf("Breakfast Fat: %.2lf \n", BreakfastFat);
    
    printf("\n\n\n======Lunch======"); //This calculates the calories and fat consumed at lunch by the user.
    do //This continues to ask the user to enter the items consumed for lunch until he or she manually exits.
    {
        printf("\n\n1. 1 Serving of Chicken\n");
        printf("2. 1 Serving of Rice\n");
        printf("3. 1 Serving of Broccoli\n");
        printf("4. Proceed to the dinner selection\n");
        printf("\nPlease select the items consumed for lunch: ");
        scanf("%d",&LunchSelection);
        
        switch(LunchSelection)
        {
            case 1:
                printf("\nPlease enter the servings of chickens consumed: ");
                scanf(" %d", &ChickenServing);
                break;
            case 2:
                printf("\nPlease enter the servings of rice consumed: ");
                scanf(" %d", &RiceServing);
                break;
            case 3:
                printf("\nPlease enter the servings of broccoli consumed: ");
                scanf(" %d", &BroccoliServing);
                break;
        }
    }while(LunchSelection != 4);
    
    LunchCalories = (double)calculation(ChickenServing, RiceServing, BroccoliServing, ChickenCalories, RiceCalories, BroccoliCalories); //This calculates the total calories consumed at lunch.
    LunchFat = (double)calculation(ChickenServing, RiceServing, BroccoliServing, ChickenFat, RiceFat, BroccoliFat); //This calculates the total fat consumed at lunch.
    printf("\nLunch Calories: %.2lf \n", LunchCalories);
    printf("Lunch Fat: %.2lf", LunchFat);
    
    
    printf("\n\n\n======Dinner======"); //This calculates the calories and fat consumed at dinner by the user.
    do //This continues to ask the user to enter the items consumed for dinner until he or she manually exits.
    {
        printf("\n\n1. 1 Serving of Pasta \n");
        printf("2. 1 Serving of Carrots\n");
        printf("3. 1 Serving of Fish\n");
        printf("4. Exit");
        printf("\n\nPlease select the items consumed for dinner: ");
        scanf("%d",&DinnerSelection);
        
        switch(DinnerSelection)
        {
            case 1:
                printf("\nPlease enter the servings of pasta consumed: ");
                scanf(" %d", &PastaServing);
                break;
            case 2:
                printf("\nPlease enter the servings of carrots consumed: ");
                scanf(" %d", &CarrotsServing);
                break;
            case 3:
                printf("\nPlease enter the servings of fish consumed: ");
                scanf(" %d", &FishServing);
                break;
        }
    }while(DinnerSelection != 4);
    
    DinnerCalories = (double)calculation(PastaServing, CarrotsServing, FishServing, PastaCalories, CarrotsCalories, FishCalories);//(PastaCalories*PastaServing)+(CarrotsCalories*CarrotsServing)+(FishCalories*FishServing); //This calculates the total calories consumed at dinner.
    DinnerFat = (double)calculation(PastaServing, CarrotsServing, FishServing, PastaFat, CarrotsFat, FishFat); //This calculates the total fat consumed at dinner.
    printf("\nDinner Calories: %.2lf \n",DinnerCalories);
    printf("Dinner Fat: %.2lf \n",DinnerFat);
    
    
    TotalCalories= (double) totalcalfat(BreakfastCalories, LunchCalories, DinnerCalories); //This calculates the total calories consumed for the day during all three meals.
    TotalFat= (double) totalcalfat(BreakfastFat, LunchFat, DinnerFat); //This calculates the total fat consumed for the day during all three meals.
    
    printf("Total Calories: %.2lf \n", TotalCalories);
    printf("Total Fat: %.2lf \n", TotalFat);
    
    
}





void OneDayFeedback(int eaten_fruits, int eaten_vegetables, int eaten_grains, int eaten_protein, int eaten_sweets, int eaten_dairy,
                   int alloted_fruits, int alloted_vegetables, int alloted_grains, int alloted_protein, int alloted_sweets, int alloted_dairy)
{
    int total_grains;
    int total_fruits;
    int total_vegetables;
    int total_protein;
    int total_sweets;
    int total_dairy;
    int day_number;
    
    float grains_avg;
    float fruits_avg;
    float vegetables_avg;
    float protein_avg;
    float sweets_avg;
    float dairy_avg;
    
    for (day_number=1; day_number<=7; day_number++)
    {
        if (eaten_grains>= alloted_grains)
            printf("You ate enough grains today.\n");
        else
            printf("You did not eat enough grains today.\n");
        if(day_number==1 || day_number == 4 || day_number==7)
            printf("Health Tip: Make sure you're getting enough whole grains.\n");
        else if(day_number==2 || day_number== 5)
            printf("Health Tip: Foods containing fiber (such as whole grains) help provide a feeling of fullness with fewer calories.\n");
        else
            printf("Health Tip: Grains are important sources of many nutrients including dietary fiber, B vitamins, and minerals.\n");
        
        if (eaten_fruits>= alloted_fruits)
            printf("You ate enough fruits today.\n");
        else
            printf("You did not eat enough fruits today.\n");
        if(day_number==1 || day_number == 4 || day_number==7)
            printf("Health Tip: Most fruits are naturally low inf fat, sodium and calories. None have cholesterol.\n");
        else if(day_number==2 || day_number== 5)
            printf("Health Tip: Diets rich in potassium may help to maintain healthy blood pressure. Try some bananas, dried peaches, or even orange juice.\n");
        else
            printf("Health Tip: Eating a diet rich in fruits may reduce risk of heart disease, obesity and type 2 diabetes, as well as protecting against certain types of cancers.\n");
        
        
        if (eaten_vegetables>= alloted_vegetables)
            printf("You ate enough vegetables today.\n");
        else
            printf("You did not eat enough vegetables today.\n");
        if(day_number==1 || day_number == 4 || day_number==7)
            printf("Health Tip: Buy fresh vegetables in season. They often cost less and are likely to be at their peak flavor.\n");
        else if(day_number==2 || day_number== 5)
            printf("Health Tip: Prepare more foods from fresh ingredients to lower sodium intake. Most sodium in the food supply comes from packaged or processed foods.\n");
        else
            printf("Health Tip: Buy canned vegetables labeled 'reduced sodium' 'low sodium' or 'no salt added'. If you want to add a little salt it will likely be less than the amount in the regular canned product.\n");
        
        
        if (eaten_dairy>= alloted_dairy)
            printf("You got enough dairy today.\n");
        else
            printf("You did not get enough dairy today.\n");
        if(day_number==1 || day_number == 4 || day_number==7)
            printf("Health Tip: Intake of dairy products is linked to improved bone health, and may reduce the risk of osteoporosis.\n");
        else if(day_number==2 || day_number== 5)
            printf("Health Tip: Milk products that are consumed in their low-fat or fat-free forms provide little or no solid fat.\n");
        else
            printf("Health Tip: If you usually drink whole milk, switch gradually to fat-free milk to lower saturated fat and calories. Try reduced fat (2%) then low-fat (1%) and finally fat-free (skim).\n");
        
        if (eaten_protein>= alloted_protein)
            printf("You got enough protein today.\n");
        else
            printf("You did not get enough protein today.\n");
        if(day_number==1 || day_number == 4 || day_number==7)
            printf("Health Tip: Try eating seafood at least twice a week as the main protein food. Look for seafood rich in omega-3 fatty acids, such as salmon, trout, and herring.\n");
        else if(day_number==2 || day_number== 5)
            printf("Health Tip: To keep meat lean trim away all of the visible fat from meats and poultry before cooking and drain off any fat that appears during cooking.\n");
        else
            printf("Health Tip: Skip of limit the breading on meat, poultry, or fish. Breading adds calories and will also cause the food to soak up more fat during frying.\n");
        
        
        printf("You ate %d servings of sweets today.\n", &eaten_sweets);
        printf("Health Tip: Make sure to keep this number low, these types of foods in excess can cause serious health damage.");
        
        total_grains= total_grains + eaten_grains;
        total_fruits= total_fruits + eaten_fruits;
        total_vegetables= total_vegetables + eaten_vegetables;
        total_protein= total_protein + eaten_protein;
        total_sweets= total_sweets + eaten_sweets;
        total_dairy= total_dairy + eaten_dairy;
    }
    grains_avg= total_grains/7;
    fruits_avg= total_fruits/7;
    vegetables_avg= total_vegetables/7;
    protein_avg= total_protein/7;
    sweets_avg= total_sweets/7;
    dairy_avg= total_dairy/7;
    
    
  
};


