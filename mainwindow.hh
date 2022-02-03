#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include "QVBoxLayout"
#include "QHBoxLayout"
#include <vector>
#include <QPushButton>
#include <gameboard.hh>
#include <square.hh>
#include <QLineEdit>
#include <utility>
#include <QRadioButton>
#include <QTimer>
#include <QLCDNumber>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_start_button_clicked();
    void on_game_button_clicked();
    void on_reset_button_clicked();
    void on_close_button_clicked();
    void on_flag_check_button_clicked();

private:
    Ui::MainWindow *ui;

    QWidget* central = new QWidget(this);
    // Vertical Box layout that contains every button in the window
    QVBoxLayout* window_layout = new QVBoxLayout(central);
    // Horizontal box layout that contains the buttons that are not
    // part of the game grid. This is added as the topmost element
    // to window_layout
    QHBoxLayout* non_grid_button_layout = new QHBoxLayout(this);

    // Initializing the UI elements that need to be accessible
    QLineEdit* seed_entry = new QLineEdit(this);
    QPushButton* start_button = new QPushButton;
    QRadioButton* flag_chosen = new QRadioButton;
    QRadioButton* open_chosen = new QRadioButton;
    QRadioButton* unsure_chosen = new QRadioButton;
    QTimer* timer = new QTimer;
    QLCDNumber* clock = new QLCDNumber;

    // generates the game grid.
    void init_game_grid();
    // generates the layout of the whole program.
    void init_window_layout();

    void init_seed_label();
    void add_seed_entry();
    void init_start_button();
    void init_reset_button();
    void init_close_button();
    // initializes every element in the second row of the main window layout
    void init_second_row();
    // increments the number displayed on the clock every second
    void on_sec_timer();
    // Synchronizes the gameboard of the ui with the gameboard object it's based
    // on. Only used to make sure all the opened squares are also open on the UI.
    void sync_game_grid();
    // Ends the game by opening all the buttons and creating a game over message
    void game_over();
    // Deletes every button in the gameboard of the UI as well as clears the
    // board_ vector
    void clear_gameboard();
    // Ends the game by opening all the squares and prints a message showing
    // the player thei score.
    void player_wins();
    // Counts the score of the player based on the time it took to beat the game
    // as well as how many flags were used
    int count_score();
    // Opens every square on the board, forcing the player to either close the
    // game or press the reset button
    void open_all_squares();
    // Adds the correct image to the button pointed to by the parameter ptr.
    void add_image(QPushButton* ptr);
    // Counts how many flags the user has misplaced.
    bool flags_in_wrong_places();

    // Returns the coordinates of the square corrseponding to the button
    // that was clicked. ptr = a pointer to the button object that
    // was clicked.
    std::pair <int, int> find_square(QObject* ptr);
    // reads and returns the seed value given by the player.
    // uses the computer time as a seed if no input is given
    int read_seed_value();

    std::vector<std::vector<QPushButton*>> game_buttons;
    GameBoard board;


};
#endif // MAINWINDOW_HH