

enum GameValue {Rock, Scissors, Paper};
enum GameResult {Win, Draw, Lose};

const char* const to_string(const enum GameValue value);
enum GameResult get_game_result(const enum GameValue player, const enum GameValue computer);
enum GameValue generate_computer_move();