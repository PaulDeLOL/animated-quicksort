#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
using namespace std;
using namespace std::this_thread;
using namespace std::chrono;


// Enumerated data type for pivot selection
enum PivotSelection {FIRST, LAST, MIDDLE};


// Three QuickSort algorithms, each having a different choice of pivot
void QuickSortFirst(vector<int> &arr, int start, int end, int max, double &total);
void QuickSortMiddle(vector<int> &arr, int start, int end, int max, double &total);
void QuickSortLast(vector<int> &arr, int start, int end, int max, double &total);

// Prints a visual representation of the array by drawing stacks of stars (*)
void PrintArray(const vector<int> &arr, int pivot, int max, double duration);



int main() {
    // Random number generator from 1 to 10
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> nums(1, 10);

    // Create an array of n random elements between 1 and 10
    // Also determine the max element to determine how high the stacks
    // of stars (*) should be
    // Change n to determine the size of the array
    vector<int> arr;
    int n = 10, max = -2147483648;
    arr.reserve(n);
    for (int i = 0; i < n; i++)
    {
        arr.push_back(nums(gen));
        if (arr[arr.size() - 1] > max)
            max = arr[arr.size() - 1];
    }

    // Select the pivot by changing selection to FIRST, MIDDLE, or LAST
    PivotSelection selection = LAST;
    double duration = 0;

    switch (selection) {
        // Print the initial unsorted array before executing the chosen algorithm
        case FIRST:
            PrintArray(arr, 0, max, 0);
            QuickSortFirst(arr, 0, arr.size() - 1, max, duration);
            break;
        case MIDDLE:
            PrintArray(arr, arr.size() / 2, max, 0);
            QuickSortMiddle(arr, 0, arr.size() - 1, max, duration);
            break;
        case LAST:
            PrintArray(arr, arr.size() - 1, max, 0);
            QuickSortLast(arr, 0, arr.size() - 1, max, duration);
            break;
    }

    cout << "Total time taken by QuickSort: " << fixed << setprecision(9) << duration << " seconds" << endl;
}



// QuickSort algorithm with pivot at the start of each sub-array
void QuickSortFirst(vector<int> &arr, int start, int end, int max, double &total)
{
    time_point<steady_clock> begin = high_resolution_clock::now();

    if (end <= start)
        return;

    int j = end + 1;
    int pivot = start;

    for (int i = end; i >= start; i--)
    {
        if (arr[i] >= arr[pivot])
        {
            j--;
            if (i < j)
            {
                if (i == pivot)
                    pivot = j;
                swap(arr[i], arr[j]);
            }
        }
    }

    // Calculates the time taken for each iteration and adds it to a running total
    time_point<steady_clock> finish = high_resolution_clock::now();
    double iteration_time = duration_cast<nanoseconds>(finish - begin).count() * 1e-9;
    total += duration_cast<nanoseconds>(finish - begin).count() * 1e-9;

    // Prints the visual representation after each iteration
    PrintArray(arr, pivot, max, iteration_time);

    QuickSortFirst(arr, start, pivot - 1, max, total);
    QuickSortFirst(arr, pivot + 1, end, max, total);
}

// QuickSort algorithm with pivot in the middle of each sub-array
void QuickSortMiddle(vector<int> &arr, int start, int end, int max, double &total)
{
    time_point<steady_clock> begin = high_resolution_clock::now();

    if (start >= end)
        return;

    int pivot = (start + end) / 2;
    int left = start;
    int right = end;

    while (left <= right)
    {
        while (arr[left] < arr[pivot])
            left++;
        while (arr[right] > arr[pivot])
            right--;
        if (left <= right)
        {
            if (right == pivot)
                pivot = left;
            else if (left == pivot)
                pivot = right;
            swap(arr[left], arr[right]);
            left++;
            right--;
        }
    }

    // Calculates the time taken for each iteration and adds it to a running total
    time_point<steady_clock> finish = high_resolution_clock::now();
    double iteration_time = duration_cast<nanoseconds>(finish - begin).count() * 1e-9;
    total += duration_cast<nanoseconds>(finish - begin).count() * 1e-9;

    // Prints the visual representation after each iteration
    PrintArray(arr, pivot, max, iteration_time);

    QuickSortMiddle(arr, start, left - 1, max, total);
    QuickSortMiddle(arr, left, end, max, total);
}

// QuickSort algorithm with pivot at the end of each sub-array
void QuickSortLast(vector<int> &arr, int start, int end, int max, double &total)
{
    time_point<steady_clock> begin = high_resolution_clock::now();

    if (start >= end)
        return;

    int j = start - 1;
    int pivot = end;

    for (int i = start; i <= end; i++)
    {
        if (arr[i] <= arr[pivot])
        {
            j++;
            if (i > j)
            {
                if (i == pivot)
                    pivot = j;
                swap(arr[i], arr[j]);
            }
        }
    }

    // Calculates the time taken for each iteration and adds it to a running total
    time_point<steady_clock> finish = high_resolution_clock::now();
    double iteration_time = duration_cast<nanoseconds>(finish - begin).count() * 1e-9;
    total += duration_cast<nanoseconds>(finish - begin).count() * 1e-9;

    // Prints the visual representation after each iteration
    PrintArray(arr, pivot, max, iteration_time);

    QuickSortLast(arr, start, pivot - 1, max, total);
    QuickSortLast(arr, pivot + 1, end, max, total);
}


// Prints a visual representation of the array by drawing stacks of stars (*)
// The pivot is represented by a red stack of stars
void PrintArray(const vector<int> &arr, int pivot, int max, double duration)
{
    // Clears the screen before printing to give the impression of a
    // frame-by-frame animation (works best if the user doesn't scroll up)
    // This is done with an ANSI escape code
    cout << "\u001b[2J";

    cout << "--------";
    for (int i = 0; i < arr.size() * 3; i++)
        cout << "-";
    cout << endl;

    for (int h = max; h > 0; h--)
    {
        cout << "      | ";
        for (int i = 0; i < arr.size(); i++)
        {
            if (i >= 10 || arr[i] == 10)
                cout << " ";
            if (h <= arr[i])
                if (i == pivot)
                    cout << "\u001b[38;5;196m*\u001b[0m "; // Red coloration is achieved with ANSI escape codes
                else
                    cout << "* ";
            else
                cout << "  ";
        }
        cout << "|" << endl;
    }

    cout << "--------";
    for (int i = 0; i < arr.size() * 3; i++)
        cout << "-";
    cout << endl;

    cout << "Index:  ";
    for (int i = 0; i < arr.size(); i++)
    {
        if (i < 10 && arr[i] == 10)
            cout << " ";
        if (i == pivot)
            cout << "\u001b[38;5;196m" << i << "\u001b[0m ";
        else
            cout << i << " ";
    }
    cout << endl;

    cout << "Value:  ";
    for (int i = 0; i < arr.size(); i++)
    {
        if (i >= 10 && arr[i] < 10)
            cout << " ";
        if (i == pivot)
            cout << "\u001b[38;5;196m" << arr[i] << "\u001b[0m ";
        else
            cout << arr[i] << " ";
    }
    cout << endl;

    // Prints the current running total for all iterations of
    // the chosen QuickSort algorithm up to this point
    cout << "Time taken for this iteration: " << fixed << setprecision(9) << duration << " seconds" << endl;

    // Delays execution of the program for fixed time in milliseconds
    // Lower the value to make the animation go faster, raise it to make it go slower
    sleep_for(milliseconds(250));
}
