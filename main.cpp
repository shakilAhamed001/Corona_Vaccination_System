#include <bits/stdc++.h>
#include <conio.h>
using namespace std;

class Patient
{
private:
    int id;
    int pass;
    char name[50];
    int age;
    int rd, rm, ry; // Registration date
    int fd_day, fd_mon, fd_year; // First Dose Date
    int sd_day, sd_mon, sd_year; // Second Dose Date

public:
    void registration();
    void showData();
    int getId();
    int getPass();
    static int random_num();
    void calculateDoseDates();
};

int Patient::random_num()
{
    srand(time(0));
    int random = rand();
    cout << "ID number = " << random << endl;
    return random;
}

int Patient::getId()
{
    return id;
}

int Patient::getPass()
{
    return pass;
}

void Patient::registration()
{
    id = random_num();
    cout << "Enter Patient Password: ";
    cin >> pass;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    while (true)
    {
        cout << "Enter Patient Name (alphabets only): ";
        cin.getline(name, 50);

        // Check if the name contains only alphabets and spaces
        bool valid = true;
        for (int i = 0; name[i] != '\0'; ++i)
        {
            if (!isalpha(name[i]) && name[i] != ' ')
            {
                valid = false;
                break;
            }
        }

        if (valid) break; // Exit the loop if the name is valid
        cout << "Error: Name must contain only alphabets. Please try again.\n";
    }

    cout << "Enter Patient Age: ";
    while (!(cin >> age) || age <= 0)
    {
        cout << "Please enter a valid number for age: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if (age < 18 || age > 120)
    {
        cout << "Error: Age must be between 18 and 120 years!\n";
        return;
    }

    time_t now = time(0);
    tm *localTime = localtime(&now);
    rd = localTime->tm_mday;
    rm = localTime->tm_mon + 1;
    ry = localTime->tm_year + 1900;

    calculateDoseDates();

    cout << "First Dose Date: " << fd_day << "/" << fd_mon << "/" << fd_year << endl;
    cout << "Second Dose Date: " << sd_day << "/" << sd_mon << "/" << sd_year << endl;
}

void Patient::calculateDoseDates()
{
    fd_day = rd;
    fd_mon = rm + 1;
    fd_year = ry;
    if (fd_mon > 12)
    {
        fd_mon -= 12;
        fd_year += 1;
    }

    sd_day = rd;
    sd_mon = rm + 2;
    sd_year = ry;
    if (sd_mon > 12)
    {
        sd_mon -= 12;
        sd_year += 1;
    }
}

void Patient::showData()
{
    cout << "\nPatient ID: " << id
         << "\nName: " << name
         << "\nAge: " << age
         << "\nRegistration Date: " << rd << "/" << rm << "/" << ry
         << "\nFirst Dose Date: " << fd_day << "/" << fd_mon << "/" << fd_year
         << "\nSecond Dose Date: " << sd_day << "/" << sd_mon << "/" << sd_year << endl;
}

void addPatient()
{
    Patient *pt = (Patient *)malloc(sizeof(Patient)); // Dynamic memory allocation

    if (pt == NULL)
    {
        cout << "Memory allocation failed!\n";
        return;
    }

    ofstream outfile("patients.data", ios::binary | ios::app); // Use `ios::app` to append data

    pt->registration();
    outfile.write(reinterpret_cast<char *>(pt), sizeof(Patient));
    outfile.close();

    cout << "Patient record created successfully!\n";

    free(pt); // Free allocated memory
}

void searchPatient()
{
    int idnum;
    cout << "Enter Patient ID: ";
    cin >> idnum;

    Patient *pt = (Patient *)malloc(sizeof(Patient)); // Dynamic memory allocation

    if (pt == NULL)
    {
        cout << "Memory allocation failed!\n";
        return;
    }

    ifstream infile("patients.data", ios::binary);
    bool found = false;

    while (infile.read(reinterpret_cast<char *>(pt), sizeof(Patient)))
    {
        if (pt->getId() == idnum)
        {
            pt->showData();
            found = true;
            break;
        }
    }

    infile.close();
    free(pt); // Free allocated memory

    if (!found)
    {
        cout << "Patient record not found!\n";
    }
}

void loginPatient()
{
    int loginId, loginPass;
    cout << "\n--- Patient Login ---\n";
    cout << "Enter Patient ID: ";
    cin >> loginId;
    cout << "Enter Patient Password: ";
    cin >> loginPass;

    Patient *pt = (Patient *)malloc(sizeof(Patient)); // Dynamic memory allocation

    if (pt == NULL)
    {
        cout << "Memory allocation failed!\n";
        return;
    }

    ifstream infile("patients.data", ios::binary);
    bool found = false;

    while (infile.read(reinterpret_cast<char *>(pt), sizeof(Patient)))
    {
        if (pt->getId() == loginId && pt->getPass() == loginPass)
        {
            cout << "\nLogin Successful!\n";
            pt->showData();
            found = true;
            break;
        }
    }

    infile.close();
    free(pt); // Free allocated memory

    if (!found)
    {
        cout << "Login failed! Invalid ID or password.\n";
    }
}

void deletePatient()
{
    int deleteId, deletePass;
    cout << "Enter Patient ID to delete: ";
    cin >> deleteId;
    cout << "Enter Patient Password: ";
    cin >> deletePass;

    Patient *pt = (Patient *)malloc(sizeof(Patient)); // Dynamic memory allocation

    if (pt == NULL)
    {
        cout << "Memory allocation failed!\n";
        return;
    }

    ifstream infile("patients.data", ios::binary);
    ofstream outfile("temp.data", ios::binary);
    bool found = false;

    while (infile.read(reinterpret_cast<char *>(pt), sizeof(Patient)))
    {
        if (pt->getId() == deleteId && pt->getPass() == deletePass)
        {
            found = true;
        }
        else
        {
            outfile.write(reinterpret_cast<char *>(pt), sizeof(Patient));
        }
    }

    infile.close();
    outfile.close();
    free(pt); // Free allocated memory

    remove("patients.data");
    rename("temp.data", "patients.data");

    if (found)
    {
        cout << "Patient record deleted successfully!\n";
    }
    else
    {
        cout << "Patient record not found!\n";
    }
}

int main()
{
    int choice;

    do
    {
        cout << "\n--- Main Menu ---\n";
        cout << "1. Patient Login\n";
        cout << "2. Add New Patient\n";
        cout << "3. Search Patient\n";
        cout << "4. Delete Patient\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        system("cls");

        switch (choice)
        {
        case 1:
            loginPatient();
            break;
        case 2:
            addPatient();
            break;
        case 3:
            searchPatient();
            break;
        case 4:
            deletePatient();
            break;
        case 5:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice!\n";
        }
    }
    while (choice != 5);

    return 0;
}
