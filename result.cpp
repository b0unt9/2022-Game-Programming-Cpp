#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
#include <windows.h>
using namespace std;

struct question {
    string question;
    int space;
    int answer;
};

struct answer {
    vector<int> answer;
};

vector<question> questions;
vector<answer> answers;

void mainMemuPage(void);
void surveyManagerMemuPage();

void surveyInputPage();

void surveyStatisticsViewPage();
void surveyQuesionAddPage();
void surveyAnswerResetPage();
void surveyAnswerTableViewPage();
void surveyAnswerExportPage();

void readQuestion();
void readAnswer();

void changeCursorPositon(int x, int y);
void printQuestion();

vector<int> split(string str, char Delimiter);
int getAnswerCount();

int main(void)
{
    mainMemuPage();
    return 0;
}

void mainMemuPage(void)
{
    int tmp;
    while (1) {
        system("cls");
        cout << "1. 설문 조사하러 가기" << endl;
        cout << "2. 설문 관리 페이지 가기" << endl;
        cout << "3. 종료" << endl;
        cout << "\n\n입력 : ";

        cin.clear();
        cin >> tmp;

        switch (tmp) {
        case 1:
            surveyInputPage();
            break;

        case 2:
            surveyManagerMemuPage();
            break;

        case 3:
            exit(0);
            break;

        default:
            cout << "잘못 된 입력 입니다" << endl;
            system("pause");
            break;
        }
    }
}

void surveyManagerMemuPage()
{
    int tmp;
    bool flag = true;
    while (flag) {
        system("cls");
        cout << "1. 설문 통계 보기" << endl;
        cout << "2. 설문 조사 질문 추가" << endl;
        cout << "3. 설문 조사 결과 초기화" << endl;
        cout << "4. 설문 조사 결과보기 (표)" << endl;
        cout << "5. 설문 조사 결과 내보내기 (CSV)" << endl;
        cout << "6. 메인으로 돌아가기" << endl;
        cout << "\n\n입력 : ";

        cin.clear();
        cin >> tmp;

        switch (tmp) {
        case 1:
            surveyStatisticsViewPage();
            break;

        case 2:
            surveyQuesionAddPage();
            break;

        case 3:
            surveyAnswerResetPage();
            break;

        case 4:
            surveyAnswerTableViewPage();
            break;

        case 5:
            surveyAnswerExportPage();
            break;

        case 6:
            flag = false;
            break;

        default:
            cout << "잘못 된 입력 입니다" << endl;
            system("pause");
            break;
        }
    }
}

// survey_input -> surveyInputPage
void surveyInputPage()
{
    int yn;

    do {
        readQuestion();
        printQuestion();

        string fileName("설문결과.txt");

        fstream resultFile;
        resultFile.open(fileName.c_str(), std::ios_base::app | std::ios_base::in);

        if (!resultFile.is_open()) {
            cerr << "설문 내용 파일(" << fileName << ")을 열 수 없음" << endl;
            system("pause");
            break;
        }

        for (int i = 0; i < questions.size(); ++i) {
            while (1) {
                int tmp;
                changeCursorPositon(questions[i].space + 7, i + 6);
                cin.clear();
                cin >> tmp;

                if (tmp >= 1 && tmp <= 5) {
                    questions[i].answer = tmp;
                    break;
                }

                system("cls");
                cout << "1~5까지만 입력 가능합니다" << endl;
                system("pause");
                changeCursorPositon(0, -1);
                printQuestion();
                cin.clear();
                cin.ignore();
            }
            resultFile << questions[i].answer;
            if (i != questions.size() - 1) {
                resultFile << ",";
            }
        }
        resultFile << "\n";

        resultFile.close();

        cout << "\n설문조사가 끝났습니다. 새로운 설문조사는 1, 메뉴로 돌아가려면 0 : ";
        cin >> yn;
    } while (yn == 1);
}

void surveyStatisticsViewPage()
{
    readQuestion();
    int answerCount = getAnswerCount();
    int questionCount = questions.size();

    system("cls");
    cout << "설문 통계" << endl;
    cout << "질문 개수 : " << questionCount << "\n답변 개수 : " << answerCount << endl;
    system("pause");
}

void surveyQuesionAddPage()
{
    bool flag = true;
    string text;
    string fileName("설문내용.txt");
    fstream questionFile;

    questionFile.open(fileName.c_str(), std::ios_base::app | std::ios_base::in);

    if (!questionFile.is_open()) {
        cerr << "설문 결과를 저장할 파일(" << fileName << ")을 열 수 없음" << endl;
    }

    while (flag) {
        int tmp;
        system("cls");
        cout << "추가할 질문 입력 : ";
        cin.clear();
        cin.ignore();
        getline(cin, text);

        questionFile << text << endl;
        cout << "질문 추가가 완료 되었습니다. 질문을 더 추가하려면 1, 이전 메뉴로 돌아가려면 0 : ";
        cin.clear();
        cin >> tmp;
        switch (tmp) {
        case 1:
            continue;

        case 0:
            flag = false;
            break;

        default:
            cout << "잘못 된 입력 입니다" << endl;
            system("pause");
            cout << "질문 추가가 완료 되었습니다. 질문을 더 추가하려면 1, 이전 메뉴로 돌아가려면 0 : ";
            cin.clear();
            cin >> tmp;
            break;
        }
    }

    questionFile.close();
}

void surveyAnswerResetPage()
{
    int tmp;

    system("cls");
    cout << "정말로 설문 결과를 모두 삭제하시겠습니까? 네: 1, 아니요: 0 : ";

    cin.clear();
    cin >> tmp;

    if (tmp == 1) {
        int res = remove("설문결과.txt");
        if (res == 0) {
            cout << "삭제 완료" << endl;
            system("pause");
        } else if (res == -1) {
            cout << "삭제 실패" << endl;
        }
    }
}

void surveyAnswerTableViewPage()
{
    struct ans {
        string question;
        int space;
        int a, b, c, d, e;
    };

    readQuestion();
    readAnswer();

    vector<ans> tmp;

    int maxText = INT_MIN;

    for (int i = 0; i < questions.size(); ++i) {
        if (questions[i].space > maxText) {
            maxText = questions[i].space + 2;
        }

        tmp.push_back({ questions[i].question, questions[i].space, 0, 0, 0, 0, 0 });
    }

    for (int i = 0; i < answers.size(); ++i) {
        for (int j = 0; j < answers[i].answer.size(); ++j) {
            if (answers[i].answer[j] == 1)
                tmp[j].a++;
            else if (answers[i].answer[j] == 2)
                tmp[j].b++;
            else if (answers[i].answer[j] == 3)
                tmp[j].c++;
            else if (answers[i].answer[j] == 4)
                tmp[j].d++;
            else if (answers[i].answer[j] == 5)
                tmp[j].e++;
        }
    }

    system("cls");
    cout << "┌";
    for (int i = 0; i < maxText; ++i) {
        cout << "─";
    }
    cout << "┬─┬─┬─┬─┬─┐" << endl;
    string head("질문");
    cout << "│" << head;
    for (int j = 0; j < maxText - head.length(); ++j) {
        cout << " ";
    }
    cout << "│1│2│3│4│5│" << endl;
    cout << "├";
    for (int j = 0; j < maxText; ++j) {
        cout << "─";
    }
    cout << "┼─┼─┼─┼─┼─┤" << endl;

    for (int i = 0; i < tmp.size(); ++i) {
        cout << "│" << tmp[i].question;
        for (int j = 0; j < maxText - tmp[i].space; ++j) {
            cout << " ";
        }
        cout << "│" << tmp[i].a << "│" << tmp[i].b << "│" << tmp[i].c << "│" << tmp[i].d << "│" << tmp[i].e << "│" << endl;
        if (i == tmp.size() - 1) {
            cout << "└";
            for (int j = 0; j < maxText; ++j) {
                cout << "─";
            }
            cout << "┴─┴─┴─┴─┴─┘" << endl;
        } else {
            cout << "├";
            for (int j = 0; j < maxText; ++j) {
                cout << "─";
            }
            cout << "┼─┼─┼─┼─┼─┤" << endl;
        }
    }

    system("pause");
}

void surveyAnswerExportPage()
{
    string fileName;
    bool flag = true;

    readQuestion();

    while (flag) {
        system("cls");
        cout << "파일 확장자를 제외한 파일 이름을 입력하시오 : ";

        cin.clear();
        cin >> fileName;
        fileName.append(".csv");

        if (access(fileName.c_str(), F_OK) == 0) {
            int tmp;
            cout << "해당 이름의 파일이 존재합니다. 덮어 씌우시겠습니까? 네: 1, 아니오: 0 : ";
            cin.clear();
            cin >> tmp;
            switch (tmp) {
            case 1: {
                int res = remove(fileName.c_str());
                if (res == 0) {
                    flag = false;
                    break;
                } else if (res == -1) {
                    cout << "덮어 쓰기 실패" << endl;
                    system("pause");
                    break;
                }
                flag = false;
                break;
            }

            case 0:
                flag = true;
                break;

            default:
                cout << "잘못 된 입력 입니다" << endl;
                system("pause");
                cout << "해당 이름의 파일이 존재합니다. 덮어 씌우시겠습니까? 네: 1, 아니오: 0 : ";
                cin.clear();
                cin >> tmp;
                break;
            }
        } else {
            flag = false;
        }
    }

    ofstream exportFile;
    exportFile.open(fileName.c_str());

    if (!exportFile.is_open()) {
        cerr << "파일 " << fileName << ")을 생성할 수 없음" << endl;
        system("pause");
    }

    for (int i = 0; i < questions.size(); ++i) {
        exportFile << questions[i].question;
        if (i == questions.size() - 1) {
            exportFile << endl;
        } else {
            exportFile << ",";
        }
    }

    string text;
    string answerFileName("설문결과.txt");
    ifstream answerFile(answerFileName.c_str());

    if (!answerFile.is_open()) {
        cerr << "설문 내용 파일(" << answerFileName << ")을 열 수 없음" << endl;
        system("pause");
    }

    while (getline(answerFile, text)) {
        exportFile << text << endl;
    }

    answerFile.close();
    exportFile.close();

    cout << "Export 완료 (출력 파일 : " << fileName << ")" << endl;
    system("pause");
}
// gotoxy -> changeCursorPositon
void changeCursorPositon(int x, int y)
{
    COORD Pos = { x - 1, y - 1 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void readQuestion()
{
    questions.clear();

    string text;
    string fileName("설문내용.txt");
    ifstream questionFile(fileName.c_str());

    if (!questionFile.is_open()) {
        cerr << "설문 내용 파일(" << fileName << ")을 열 수 없음" << endl;
        system("pause");
    }

    while (getline(questionFile, text)) {
        question tmp = { text, (int)text.length(), 0 };
        questions.push_back(tmp);
    }

    questionFile.close();
}

void printQuestion()
{
    system("cls");
    cout << "각 설문에 대해 문항의 답을 입력하고 Enter를 입력하시오." << endl;
    cout << "문항의 답은 숫자만 사용합니다.\n\n";
    cout << "1: 매우 불만 2: 불만 3: 보통 4: 만족 5: 매우 만족\n\n";

    for (int i = 0; i < questions.size(); ++i) {
        if (questions[i].answer != 0) {
            cout << i + 1 << ") " << questions[i].question << " : " << questions[i].answer << endl;
        } else {
            cout << i + 1 << ") " << questions[i].question << " : " << endl;
        }
    }
}

void readAnswer()
{
    answers.clear();

    string text;
    string fileName("설문결과.txt");
    ifstream answerFile(fileName.c_str());

    if (!answerFile.is_open()) {
        cerr << "설문 결과 파일(" << fileName << ")을 열 수 없음" << endl;
        system("pause");
    }

    while (getline(answerFile, text)) {
        vector<int> result = split(text, ',');
        answers.push_back({ result });
    }
}

vector<int> split(string str, char Delimiter)
{
    istringstream iss(str);
    string buffer;

    vector<int> result;

    while (getline(iss, buffer, Delimiter)) {
        result.push_back(stoi(buffer));
    }

    return result;
}

int getAnswerCount()
{
    string text;
    string fileName("설문결과.txt");
    int cnt = 0;

    ifstream file(fileName.c_str());

    if (!file.is_open()) {
        cerr << "설문 내용 파일(" << fileName << ")을 열 수 없음" << endl;
        system("pause");
    }

    while (getline(file, text)) {
        cnt++;
    }

    file.close();

    return cnt;
}
