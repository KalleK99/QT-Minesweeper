#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "QGridLayout"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include <QLabel>
#include <QString>
#include <QMessageBox>
#include <QBrush>
#include <QPalette>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init_window_layout();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_start_button_clicked()
{
    int seed = read_seed_value();
    board.init(seed);

    QGridLayout* game_grid = new QGridLayout();

    for (int i = 1; i <= BOARD_SIDE; ++i)
    {
        std::vector<QPushButton*> line;
        for (int j = 1; j <= BOARD_SIDE; ++j)
        {
            QPushButton* game_button = new QPushButton;
            game_button->setFixedHeight(100);
            game_button->setFixedWidth(100);
            connect(game_button, &QPushButton::clicked,
                    this, &MainWindow::on_game_button_clicked);

            game_grid->addWidget(game_button, i, j);
            line.push_back(game_button);
        }
        game_buttons.push_back(line);
    }

    window_layout->addLayout(game_grid);

    start_button->setEnabled(false);

    timer->start(1000);
}

void MainWindow::on_game_button_clicked()
{
    // recognizes which button was clicked
    QPushButton* pressed_btn = static_cast<QPushButton*>(sender());
    std::pair <int, int> btn_coords = find_square(pressed_btn);
    int x = btn_coords.first;
    int y = btn_coords.second;
    Square square = board.getSquare(x, y);

    if (open_chosen->isChecked())
    {
        if (square.hasFlag())
        {
            square.removeFlag();
            board.setSquare(square, x, y);
            pressed_btn->setIcon(QIcon());
        }
        else if (square.hasQuestionMark())
        {
            square.removeQuestionMark();
            board.setSquare(square, x, y);
            pressed_btn->setIcon(QIcon());
        }
        else
        {
            if (!square.open())
            {
                game_over();
            }
            else
            {
                int adj_mines = square.countAdjacent();
                if (adj_mines != 0)
                {
                    add_image(pressed_btn);
                    pressed_btn->setEnabled(false);
                }
                else
                {
                    // if there are 0 adjacent mines, then more squares were
                    // opened on the gameboard and the UI is out of sync
                    sync_game_grid();
                }
            }
        }
        if (board.isGameOver())
        {
            player_wins();
        }
    }
    else if (flag_chosen->isChecked())
    {
        square.addFlag();
        board.setSquare(square, x, y);
        add_image(pressed_btn);
    }
    else if (unsure_chosen->isChecked())
    {
        square.addQuestionMark();
        board.setSquare(square, x, y);
        add_image(pressed_btn);
    }
    else
    {
        QMessageBox* alert = new QMessageBox();
        alert->setText(
            "Please open a square or place a flag or a question mark");
        alert->exec();
    }
}

void MainWindow::on_reset_button_clicked()
{
    // Deletes the game_grid layout
    QLayoutItem* child = window_layout->takeAt(2);
    window_layout->removeItem(child);
    delete child->widget();
    delete child;

    clear_gameboard();
    // the start button can be clicked again without issues
    start_button->setEnabled(true);

    timer->stop();
    clock->display(0);
}

void MainWindow::on_close_button_clicked()
{
    close();
}

void MainWindow::on_flag_check_button_clicked()
{
    QString info_text;
    if (flags_in_wrong_places())
    {
        info_text = "You have misplaced a flag.";
    }
    else
    {
        info_text = "No misplaced flags so far.";
    }

    QMessageBox* info = new QMessageBox();
    info->setText(info_text);
    info->exec();
}




void MainWindow::init_window_layout()
{
    init_seed_label();
    add_seed_entry();
    init_start_button();
    init_reset_button();
    init_close_button();


    window_layout->addLayout(non_grid_button_layout);
    setCentralWidget(central);

    init_second_row();
    connect(timer, &QTimer::timeout, this, &MainWindow::on_sec_timer);

}


void MainWindow::init_seed_label()
{
    QLabel* seed_label = new QLabel(this);
    seed_label->setText("Enter a game seed (optional):");
    non_grid_button_layout->addWidget(seed_label);
}

void MainWindow::add_seed_entry()
{
    seed_entry->setFixedWidth(150);
    non_grid_button_layout->addWidget(seed_entry);
}

void MainWindow::init_start_button()
{
    start_button->setText("Start");
    connect(start_button, &QPushButton::clicked,
            this, &MainWindow::on_start_button_clicked);
    non_grid_button_layout->addWidget(start_button);
}

void MainWindow::init_reset_button()
{
    QPushButton* reset_button = new QPushButton;
    reset_button->setText("Reset");
    connect(reset_button, &QPushButton::clicked,
            this, &MainWindow::on_reset_button_clicked);
    non_grid_button_layout->addWidget(reset_button);
}

void MainWindow::init_close_button()
{
    QPushButton* close_button = new QPushButton;
    close_button->setText("Close");
    non_grid_button_layout->addWidget(close_button);
    connect(close_button, &QPushButton::clicked,
            this, &MainWindow::on_close_button_clicked);
}

void MainWindow::init_second_row()
{
    QHBoxLayout* radio_buttons = new QHBoxLayout();

    QPushButton* flag_check_button = new QPushButton;
    connect(flag_check_button, &QPushButton::clicked,
            this, &MainWindow::on_flag_check_button_clicked);

    flag_chosen->setText("Place a flag");
    open_chosen->setText("Open a square");
    unsure_chosen->setText("Place a question mark");
    flag_check_button->setText("Check flag accuracy");
    clock->setMaximumHeight(30);
    clock->setMaximumWidth(100);

    radio_buttons->addWidget(open_chosen);
    radio_buttons->addWidget(flag_chosen);
    radio_buttons->addWidget(unsure_chosen);
    radio_buttons->addWidget(flag_check_button);
    radio_buttons->addWidget(clock);

    window_layout->addLayout(radio_buttons);

}

void MainWindow::on_sec_timer()
{
    int seconds = clock->intValue();
    ++seconds;
    clock->display(seconds);
}

void MainWindow::sync_game_grid()
{
    std::vector<std::vector<Square>> board_vec = board.getBoard_();
    QBrush brush(Qt::gray);
    QPalette palette;
    palette.setBrush(QPalette::Button, brush);
    for (size_t i = 0; i < board_vec.size(); ++i)
    {
        for (size_t j = 0; j < board_vec.size(); ++j)
        {
            if (board_vec.at(i).at(j).isOpen())
            {
                game_buttons.at(i).at(j)->setPalette(palette);
                game_buttons.at(i).at(j)->setEnabled(false);
            }
        }
    }
}

void MainWindow::game_over()
{
    timer->stop();
    open_all_squares();
    QMessageBox* alert = new QMessageBox();
    alert->setText("KABOOM! You hit a mine, game over!");
    alert->exec();
}

void MainWindow::clear_gameboard()
{
    std::vector<std::vector<Square>> board_vec = board.getBoard_();
    for (size_t i = 0; i < game_buttons.size(); ++i)
    {
        for (size_t j = 0; j < game_buttons.size(); ++j)
        {
            delete game_buttons.at(i).at(j);
        }
    }
    game_buttons.clear();
    board.emptyBoard();
}

void MainWindow::player_wins()
{
    open_all_squares();
    timer->stop();
    QString time = QString::number(clock->intValue());
    QString score = QString::number(count_score());
    QString win_message = "You won! It took you " + time
                          + " seconds to beat the game!" + "\n"
                            + "Final score: " + score;
    QMessageBox* victory = new QMessageBox();
    victory->setText(win_message);
    victory->exec();
}

int MainWindow::count_score()
{
    int denominator = clock->intValue() + board.count_flags();
    int score = 1000 / denominator;

    return score;
}

void MainWindow::open_all_squares()
{
    auto board_vec = board.getBoard_();
    for (size_t i = 0; i < BOARD_SIDE; ++i)
    {
        for (size_t j = 0; j < BOARD_SIDE; ++j)
        {
            auto square = board_vec.at(i).at(j);
            add_image(game_buttons.at(i).at(j));
            game_buttons.at(i).at(j)->setEnabled(false);
        }
    }
}

void MainWindow::add_image(QPushButton *ptr)
{
    auto coords = find_square(ptr);
    int x = coords.first;
    int y = coords.second;
    auto square = board.getSquare(x, y);

    const std::string PREFIX(":/");
    const std::string SUFFIX(".png");

    std::string filename;
    std::string number = std::to_string(square.countAdjacent());
    if (square.hasFlag())
    {
        filename = PREFIX + "flag" + SUFFIX;
    }
    else if (square.hasQuestionMark())
    {
        filename = PREFIX + "question_mark" + SUFFIX;
    }
    else if (square.hasMine())
    {
        filename = PREFIX + "mine" + SUFFIX;
    }
    else
    {

        filename = PREFIX + number + SUFFIX;
    }

    QPixmap image(QString::fromStdString(filename));

    ptr->setIcon(QIcon(image));
    ptr->setIconSize(QSize(100, 100));
}


bool MainWindow::flags_in_wrong_places()
{
    auto board_vec = board.getBoard_();
    for (size_t i = 0; i < BOARD_SIDE; ++i)
    {
        for (size_t j = 0; j < BOARD_SIDE; ++j)
        {
            Square square = board_vec.at(i).at(j);
            if (square.hasFlag() and not square.hasMine()){
                return true;
            }
        }
    }
    return false;
}


std::pair<int, int> MainWindow::find_square(QObject* ptr)
{
    std::pair <int, int> coords;
    for (size_t i = 0; i < BOARD_SIDE; ++i)
    {
        for (size_t j = 0; j < BOARD_SIDE; ++j)
        {
            if (game_buttons.at(i).at(j) == ptr)
            {
                coords = {static_cast<int>(j), static_cast<int>(i)};
            }
        }
    }
    return coords;
}

int MainWindow::read_seed_value()
{
    if (seed_entry->text() == "") {
        return time(NULL);
    }
    else {
        return seed_entry->text().toInt();
    }
}