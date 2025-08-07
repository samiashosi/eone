#include <bits/stdc++.h>
using namespace std;

#ifdef _WIN32
#include <windows.h>
#endif

struct Expense {
    string name;
    double cost;
    int priority;
    string category;
    string date;
};

struct FinancialGoal {
    string description;
    int deadline; // month (1 to N)
    int benefit;
};

bool compareByBenefit(FinancialGoal a, FinancialGoal b) {
    return a.benefit > b.benefit;
}

bool compareExpenses(const Expense& a, const Expense& b) {
    if (a.category == b.category)
        return a.priority > b.priority;
    return a.category < b.category;
}

vector<pair<int, FinancialGoal>> scheduleGoals(vector<FinancialGoal>& goals, int maxMonths) {
    sort(goals.begin(), goals.end(), compareByBenefit);
    vector<bool> slots(maxMonths, false);
    vector<pair<int, FinancialGoal>> result;

    for (auto& goal : goals) {
        for (int m = min(maxMonths, goal.deadline) - 1; m >= 0; m--) {
            if (!slots[m]) {
                slots[m] = true;
                result.push_back({m + 1, goal});
                break;
            }
        }
    }
    return result;
}

vector<Expense> knapsackSelection(vector<Expense>& expenses, double budget, double& totalPriority, double& totalCostUsed) {
    int n = expenses.size();
    int scaledBudget = static_cast<int>(budget * 100 + 0.5);

    vector<vector<int>> dp(n + 1, vector<int>(scaledBudget + 1, 0));

    for (int i = 1; i <= n; i++) {
        int cost = static_cast<int>(expenses[i - 1].cost * 100 + 0.5);
        int priority = expenses[i - 1].priority;

        for (int w = 0; w <= scaledBudget; w++) {
            if (cost > w) {
                dp[i][w] = dp[i - 1][w];
            } else {
                dp[i][w] = max(dp[i - 1][w], dp[i - 1][w - cost] + priority);
            }
        }
    }

    vector<Expense> selected;
    int w = scaledBudget;
    for (int i = n; i > 0 && w > 0; i--) {
        int cost = static_cast<int>(expenses[i - 1].cost * 100 + 0.5);
        if (dp[i][w] != dp[i - 1][w]) {
            selected.push_back(expenses[i - 1]);
            w -= cost;
        }
    }

    totalPriority = dp[n][scaledBudget];
    totalCostUsed = 0;
    for (auto& e : selected)
        totalCostUsed += e.cost;

    sort(selected.begin(), selected.end(), [](const Expense& a, const Expense& b) {
        return (a.priority / a.cost) > (b.priority / b.cost);
    });

    return selected;
}

vector<FinancialGoal> generateGoalsFromExpenses(const vector<Expense>& expenses) {
    vector<FinancialGoal> goals;
    for (auto& e : expenses) {
        FinancialGoal g;
        g.description = "Goal: " + e.name + " (" + e.category + ")";
        g.deadline = 6;
        g.benefit = e.priority;
        goals.push_back(g);
    }
    return goals;
}

string getCurrentDate() {
    time_t t = time(0);
    tm* now = localtime(&t);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d", now);
    return string(buf);
}

void saveToFile(vector<Expense>& expenses) {
    ofstream file("expenses.txt", ios::trunc);

    file << "********** Personal Finance Budget Planner **********\n";
    file << "-------------------- Expense Records --------------------\n";
    file << left << setw(15) << "Name"
         << "| " << setw(10) << "Cost"
         << "| " << setw(8) << "Priority"
         << "| " << setw(12) << "Category"
         << "| " << setw(10) << "Date" << "\n";
    file << string(60, '-') << "\n";

    for (auto& e : expenses) {
        file << left << setw(15) << e.name
             << "| " << setw(10) << fixed << setprecision(2) << e.cost
             << "| " << setw(8) << e.priority
             << "| " << setw(12) << e.category
             << "| " << setw(10) << e.date << "\n";
    }

    file.close();
    cout << "\nExpenses saved to expenses.txt.\n";
}

void saveRecommendedExpenses(const vector<Expense>& selected) {
    ofstream file("recommended_expenses.txt", ios::trunc);

    file << "********** Recommended Expenses Based on Knapsack Optimization **********\n";
    file << left << setw(15) << "Name"
         << "| " << setw(10) << "Cost"
         << "| " << setw(8) << "Priority"
         << "| " << setw(12) << "Category"
         << "| " << setw(10) << "Date" << "\n";
    file << string(60, '-') << "\n";

    for (const auto& e : selected) {
        file << left << setw(15) << e.name
             << "| " << setw(10) << fixed << setprecision(2) << e.cost
             << "| " << setw(8) << e.priority
             << "| " << setw(12) << e.category
             << "| " << setw(10) << e.date << "\n";
    }

    file.close();
    cout << "\nRecommended expenses saved to recommended_expenses.txt.\n";
}

void saveScheduledGoalsToFile(const vector<pair<int, FinancialGoal>>& scheduled) {
    ofstream file("scheduled_goals.txt", ios::trunc);

    file << "********** Scheduled Financial Goals **********\n";
    file << left << setw(10) << "Month"
         << setw(40) << "Goal Description"
         << setw(10) << "Benefit" << "\n";
    file << string(60, '-') << "\n";

    for (auto& entry : scheduled) {
        file << left << setw(10) << entry.first
             << setw(40) << entry.second.description
             << setw(10) << entry.second.benefit << "\n";
    }

    file.close();
    cout << "\nScheduled goals saved to scheduled_goals.txt.\n";
}


void loadFromFile(vector<Expense>& expenses) {
    ifstream file("expenses.txt");
    string line;
    int lineCount = 0;

    while (getline(file, line)) {
        lineCount++;

        if (lineCount <= 4 || line.empty()) continue;

        Expense e;
        try {
            e.name = line.substr(0, 15);
            e.name = e.name.substr(0, e.name.find_last_not_of(" ") + 1);

            string costStr = line.substr(18, 10);
            e.cost = stod(costStr);

            string priorityStr = line.substr(31, 8);
            e.priority = stoi(priorityStr);

            e.category = line.substr(42, 12);
            e.category = e.category.substr(0, e.category.find_last_not_of(" ") + 1);

            e.date = line.substr(57, 10);

            expenses.push_back(e);
        } catch (...) {
            continue;
        }
    }

    file.close();
}

void categorySummary(vector<Expense>& expenses) {
    unordered_map<string, double> summary;
    for (auto& e : expenses) {
        summary[e.category] += e.cost;
    }

    cout << "\n--- Expense Summary by Category ---\n";
    for (auto& s : summary) {
        cout << s.first << ": " << fixed << setprecision(2) << s.second << "\n";
    }
}

void displayExpenses(vector<Expense>& expenses) {
    cout << "\n--- Current Expenses ---\n";
    cout << left << setw(15) << "Name"
         << setw(10) << "Cost"
         << setw(10) << "Priority"
         << setw(15) << "Category"
         << setw(15) << "Date" << "\n";
    cout << string(65, '-') << "\n";

    for (auto& e : expenses) {
        cout << left << setw(15) << e.name
             << setw(10) << fixed << setprecision(2) << e.cost
             << setw(10) << e.priority
             << setw(15) << e.category
             << setw(15) << e.date << endl;
    }
}

void addExpense(vector<Expense>& expenses) {
    Expense e;
    cout << "\nEnter Expense Details:\n";
    cout << "Name: ";
    cin >> e.name;
    cout << "Cost: ";
    cin >> e.cost;
    cout << "Priority: ";
    cin >> e.priority;
    cout << "Category (Essential/Leisure/Health/Education/Transport): ";
    cin >> e.category;
    e.date = getCurrentDate();
    expenses.push_back(e);

    saveToFile(expenses);
}

void searchByCategory(vector<Expense>& expenses) {
    string cat;
    cout << "Enter category to search: ";
    cin >> cat;
    cout << "\nExpenses in category \"" << cat << "\":\n";
    cout << left << setw(15) << "Name"
         << setw(10) << "Cost"
         << setw(10) << "Priority"
         << setw(15) << "Category"
         << setw(15) << "Date" << "\n";
    cout << string(65, '-') << "\n";

    bool found = false;
    for (auto& e : expenses) {
        if (e.category == cat) {
            cout << left << setw(15) << e.name
                 << setw(10) << fixed << setprecision(2) << e.cost
                 << setw(10) << e.priority
                 << setw(15) << e.category
                 << setw(15) << e.date << endl;
            found = true;
        }
    }
    if (!found) cout << "No expenses found in this category.\n";
}

#ifdef _WIN32
void setConsoleColor(int colorCode) {
    static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)colorCode);
}
#else
void setConsoleColor(int) {}
#endif

void printColoredHeader(const string& text, int color = 14) {
    setConsoleColor(color);
    cout << text << "\n";
    setConsoleColor(7);  // Reset color
}

void printWelcomeBox() {
    int width = 80;
    string border(width, '=');
    printColoredHeader(border, 14);
    printColoredHeader("                 WELCOME TO THE FINANCIAL BUDGET PLANNER   ", 11);
    printColoredHeader(border, 14);
    cout << "\n\n";
}

int main() {
    printWelcomeBox();

    vector<Expense> expenses;
    loadFromFile(expenses);

    double budget;
    cout << "Enter total budget: ";
    cin >> budget;
    if (budget > 1e6) {
        cout << "Budget too large! Please enter a value below 1,000,000.\n";
        return 0;
    }

    int choice;
    do {
        printColoredHeader("\n--- Personal Finance Budget Planner ---", 11);

        setConsoleColor(10); cout << "1. "; setConsoleColor(7); cout << "Add Expense\n";
        setConsoleColor(10); cout << "2. "; setConsoleColor(7); cout << "View Expenses\n";
        setConsoleColor(10); cout << "3. "; setConsoleColor(7); cout << "Sort Expenses\n";
        setConsoleColor(10); cout << "4. "; setConsoleColor(7); cout << "Financial Goals Scheduling (Job Sequencing)\n";
        setConsoleColor(10); cout << "5. "; setConsoleColor(7); cout << "Knapsack Optimization\n";
        setConsoleColor(10); cout << "6. "; setConsoleColor(7); cout << "Category Summary\n";
        setConsoleColor(10); cout << "7. "; setConsoleColor(7); cout << "Search by Category\n";
        setConsoleColor(10); cout << "8. "; setConsoleColor(7); cout << "Save & Exit\n";

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                printColoredHeader("\n>> Add Expense <<", 10);
                addExpense(expenses);
                break;
            case 2:
                printColoredHeader("\n>> View Expenses <<", 13); // Light Magenta color here
                displayExpenses(expenses);
                break;
            case 3:
                sort(expenses.begin(), expenses.end(), compareExpenses);
                printColoredHeader("\nExpenses sorted by category and priority.\n", 13);
                break;
            case 4: {
                printColoredHeader("\n>> Financial Goals Scheduling <<", 14);
                cout << "Choose Financial Goal input method:\n";
                cout << "1. Manual Input\n";
                cout << "2. Auto-generate from Expenses\n";
                int method;
                cin >> method;

                vector<FinancialGoal> goals;

                if (method == 1) {
                    int numGoals;
                    cout << "\nEnter number of financial goals: ";
                    cin >> numGoals;
                    goals.resize(numGoals);

                    for (int i = 0; i < numGoals; i++) {
                        cin.ignore();
                        cout << "Goal " << i + 1 << " Description: ";
                        getline(cin, goals[i].description);
                        cout << "Deadline month (1 to N): ";
                        cin >> goals[i].deadline;
                        cout << "Benefit (priority score): ";
                        cin >> goals[i].benefit;
                    }
                } else if (method == 2) {
                    goals = generateGoalsFromExpenses(expenses);
                    cout << "\nAuto-generated " << goals.size() << " goals from expenses.\n";
                } else {
                    cout << "Invalid choice.\n";
                    break;
                }

                int maxMonths;
                cout << "\nEnter total available months to schedule goals: ";
                cin >> maxMonths;

                vector<pair<int, FinancialGoal>> scheduled = scheduleGoals(goals, maxMonths);

                printColoredHeader("\n--- Scheduled Financial Goals ---", 11);

                cout << left << setw(10) << "Month"
                     << setw(40) << "Goal Description"
                     << setw(10) << "Benefit" << endl;
                cout << string(60, '-') << endl;

                int totalBenefit = 0;
                for (auto& entry : scheduled) {
                    cout << left << setw(10) << entry.first
                         << setw(40) << entry.second.description
                         << setw(10) << entry.second.benefit << endl;
                    totalBenefit += entry.second.benefit;
                }
                cout << "\nTotal Benefit: " << totalBenefit << endl;
                  // Save scheduled goals to file
                  saveScheduledGoalsToFile(scheduled);
                break;
            }
            case 5: {
                if (expenses.empty()) {
                    cout << "No expenses to optimize.\n";
                    break;
                }

                double totalPriority = 0, totalCostUsed = 0;
                vector<Expense> selected = knapsackSelection(expenses, budget, totalPriority, totalCostUsed);

                printColoredHeader("\n--- Recommended Expenses within Budget ---", 11);

                cout << left << setw(15) << "Name"
                     << setw(10) << "Cost"
                     << setw(10) << "Priority"
                     << setw(15) << "Category"
                     << setw(15) << "Date" << "\n";
                cout << string(65, '-') << "\n";

                for (auto& e : selected) {
                    cout << left << setw(15) << e.name
                         << setw(10) << fixed << setprecision(2) << e.cost
                         << setw(10) << e.priority
                         << setw(15) << e.category
                         << setw(15) << e.date << "\n";
                }

                cout << "\nTotal Priority: " << totalPriority << endl;
                cout << "Total Cost Used: " << fixed << setprecision(2) << totalCostUsed << endl;
                cout << "Remaining Budget: " << fixed << setprecision(2) << (budget - totalCostUsed) << endl;

                saveRecommendedExpenses(selected);
                break;
            }
            case 6:
                printColoredHeader("\n--- Expense Summary by Category ---", 11);
                categorySummary(expenses);
                break;
            case 7:
                printColoredHeader("\n>> Search by Category <<", 13);
                searchByCategory(expenses);
                break;
            case 8:
                saveToFile(expenses);
                printColoredHeader("\nExiting program.\n", 14);
                break;
            default:
                printColoredHeader("\nInvalid choice. Try again.\n", 12);
        }
    } while (choice != 8);

    return 0;
}

