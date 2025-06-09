#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <random>
#include <algorithm>
#include <limits>
#include <sstream>

// Forward declarations
class Player;
class Team;
class League;

// Utility functions for input validation and display
namespace Utility {
    // Clear the console screen
    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    // Validate integer input within a range
    int getIntInput(int min, int max) {
        int input;
        while (true) {
            std::cin >> input;
            if (std::cin.fail() || input < min || input > max) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
            } else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return input;
            }
        }
    }

    // Get string input with validation
    std::string getStringInput() {
        std::string input;
        while (true) {
            std::getline(std::cin, input);
            if (input.empty()) {
                std::cout << "Input cannot be empty. Please try again: ";
            } else {
                return input;
            }
        }
    }

    // Display a centered title
    void displayTitle(const std::string& title) {
        int width = 60;
        std::cout << std::string(width, '*') << std::endl;
        std::cout << std::setw((width + title.length()) / 2) << title << std::endl;
        std::cout << std::string(width, '*') << std::endl;
    }

    // For formatting tables
    std::string centerText(const std::string& text, int width) {
        int padding = width - text.length();
        int leftPadding = padding / 2;
        int rightPadding = padding - leftPadding;
        return std::string(leftPadding, ' ') + text + std::string(rightPadding, ' ');
    }
}

// Enum for player positions
enum class Position {
    QUARTERBACK,
    RUNNING_BACK,
    WIDE_RECEIVER,
    TIGHT_END,
    KICKER,
    DEFENSE
};

// Convert Position enum to string
std::string positionToString(Position pos) {
    switch (pos) {
        case Position::QUARTERBACK: return "QB";
        case Position::RUNNING_BACK: return "RB";
        case Position::WIDE_RECEIVER: return "WR";
        case Position::TIGHT_END: return "TE";
        case Position::KICKER: return "K";
        case Position::DEFENSE: return "DEF";
        default: return "Unknown";
    }
}

// Player class to store player information and statistics
class Player {
private:
    int id;
    std::string name;
    Position position;
    std::string team;
    
    // Player statistics
    int gamesPlayed;
    
    // QB stats
    int passingYards;
    int passingTouchdowns;
    int interceptions;
    
    // RB/WR/TE stats
    int rushingYards;
    int rushingTouchdowns;
    int receivingYards;
    int receivingTouchdowns;
    
    // K stats
    int fieldGoals;
    int extraPoints;
    
    // DEF stats
    int sacks;
    int defensiveInterceptions;
    int defensiveTouchdowns;
    
    // Fantasy points
    double fantasyPoints;

public:
    Player(int id, const std::string& name, Position position, const std::string& team)
        : id(id), name(name), position(position), team(team),
          gamesPlayed(0), passingYards(0), passingTouchdowns(0), interceptions(0),
          rushingYards(0), rushingTouchdowns(0), receivingYards(0), receivingTouchdowns(0),
          fieldGoals(0), extraPoints(0), sacks(0), defensiveInterceptions(0), defensiveTouchdowns(0),
          fantasyPoints(0.0) {}
    
    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    Position getPosition() const { return position; }
    std::string getTeam() const { return team; }
    double getFantasyPoints() const { return fantasyPoints; }
    
    // Simulate a game performance based on position
    void simulateGamePerformance() {
        gamesPlayed++;
        
        // Random engine
        std::random_device rd;
        std::mt19937 gen(rd());
        
        // Reset game stats before simulation
        double gamePoints = 0.0;
        
        switch (position) {
            case Position::QUARTERBACK: {
                // Passing yards (150-400)
                std::uniform_int_distribution<> passYards(150, 400);
                int gamePassingYards = passYards(gen);
                passingYards += gamePassingYards;
                gamePoints += gamePassingYards * 0.04; // 0.04 points per passing yard
                
                // Passing TDs (0-4)
                std::uniform_int_distribution<> passTDs(0, 4);
                int gamePassingTDs = passTDs(gen);
                passingTouchdowns += gamePassingTDs;
                gamePoints += gamePassingTDs * 4.0; // 4 points per TD
                
                // Interceptions (0-3)
                std::uniform_int_distribution<> ints(0, 3);
                int gameInterceptions = ints(gen);
                interceptions += gameInterceptions;
                gamePoints -= gameInterceptions * 2.0; // -2 points per interception
                
                // Rushing yards for QB (0-50)
                std::uniform_int_distribution<> qbRush(0, 50);
                int gameRushingYards = qbRush(gen);
                rushingYards += gameRushingYards;
                gamePoints += gameRushingYards * 0.1; // 0.1 points per rushing yard
                
                // Rushing TDs (0-1)
                std::uniform_int_distribution<> qbRushTDs(0, 1);
                int gameRushingTDs = qbRushTDs(gen);
                rushingTouchdowns += gameRushingTDs;
                gamePoints += gameRushingTDs * 6.0; // 6 points per rushing TD
                break;
            }
            case Position::RUNNING_BACK: {
                // Rushing yards (30-150)
                std::uniform_int_distribution<> rushYards(30, 150);
                int gameRushingYards = rushYards(gen);
                rushingYards += gameRushingYards;
                gamePoints += gameRushingYards * 0.1; // 0.1 points per rushing yard
                
                // Rushing TDs (0-2)
                std::uniform_int_distribution<> rushTDs(0, 2);
                int gameRushingTDs = rushTDs(gen);
                rushingTouchdowns += gameRushingTDs;
                gamePoints += gameRushingTDs * 6.0; // 6 points per rushing TD
                
                // Receiving yards (0-50)
                std::uniform_int_distribution<> recYards(0, 50);
                int gameReceivingYards = recYards(gen);
                receivingYards += gameReceivingYards;
                gamePoints += gameReceivingYards * 0.1; // 0.1 points per receiving yard
                
                // Receiving TDs (0-1)
                std::uniform_int_distribution<> recTDs(0, 1);
                int gameReceivingTDs = recTDs(gen);
                receivingTouchdowns += gameReceivingTDs;
                gamePoints += gameReceivingTDs * 6.0; // 6 points per receiving TD
                break;
            }
            case Position::WIDE_RECEIVER: {
                // Receiving yards (20-150)
                std::uniform_int_distribution<> recYards(20, 150);
                int gameReceivingYards = recYards(gen);
                receivingYards += gameReceivingYards;
                gamePoints += gameReceivingYards * 0.1; // 0.1 points per receiving yard
                
                // Receiving TDs (0-2)
                std::uniform_int_distribution<> recTDs(0, 2);
                int gameReceivingTDs = recTDs(gen);
                receivingTouchdowns += gameReceivingTDs;
                gamePoints += gameReceivingTDs * 6.0; // 6 points per receiving TD
                break;
            }
            case Position::TIGHT_END: {
                // Receiving yards (10-100)
                std::uniform_int_distribution<> recYards(10, 100);
                int gameReceivingYards = recYards(gen);
                receivingYards += gameReceivingYards;
                gamePoints += gameReceivingYards * 0.1; // 0.1 points per receiving yard
                
                // Receiving TDs (0-1)
                std::uniform_int_distribution<> recTDs(0, 1);
                int gameReceivingTDs = recTDs(gen);
                receivingTouchdowns += gameReceivingTDs;
                gamePoints += gameReceivingTDs * 6.0; // 6 points per receiving TD
                break;
            }
            case Position::KICKER: {
                // Field goals (0-5)
                std::uniform_int_distribution<> fg(0, 5);
                int gameFieldGoals = fg(gen);
                fieldGoals += gameFieldGoals;
                gamePoints += gameFieldGoals * 3.0; // 3 points per field goal
                
                // Extra points (1-5)
                std::uniform_int_distribution<> xp(1, 5);
                int gameExtraPoints = xp(gen);
                extraPoints += gameExtraPoints;
                gamePoints += gameExtraPoints * 1.0; // 1 point per extra point
                break;
            }
            case Position::DEFENSE: {
                // Sacks (0-5)
                std::uniform_int_distribution<> sackDist(0, 5);
                int gameSacks = sackDist(gen);
                sacks += gameSacks;
                gamePoints += gameSacks * 1.0; // 1 point per sack
                
                // Interceptions (0-3)
                std::uniform_int_distribution<> intDist(0, 3);
                int gameDefInterceptions = intDist(gen);
                defensiveInterceptions += gameDefInterceptions;
                gamePoints += gameDefInterceptions * 2.0; // 2 points per interception
                
                // Defensive TDs (0-1)
                std::uniform_int_distribution<> defTD(0, 1);
                int gameDefTDs = defTD(gen);
                defensiveTouchdowns += gameDefTDs;
                gamePoints += gameDefTDs * 6.0; // 6 points per defensive TD
                
                // Points allowed (0-35)
                std::uniform_int_distribution<> ptsAllowed(0, 35);
                int pointsAllowed = ptsAllowed(gen);
                
                // Points based on points allowed
                if (pointsAllowed == 0) gamePoints += 10.0;
                else if (pointsAllowed <= 6) gamePoints += 7.0;
                else if (pointsAllowed <= 13) gamePoints += 4.0;
                else if (pointsAllowed <= 20) gamePoints += 1.0;
                else if (pointsAllowed <= 27) gamePoints += 0.0;
                else if (pointsAllowed <= 34) gamePoints -= 1.0;
                else gamePoints -= 4.0;
                break;
            }
        }
        
        // Update total fantasy points
        fantasyPoints += gamePoints;
    }
    
    // Display player information
    void displayInfo() const {
        std::cout << std::left << std::setw(4) << id << " | "
                  << std::setw(20) << name << " | "
                  << std::setw(5) << positionToString(position) << " | "
                  << std::setw(15) << team << " | "
                  << std::fixed << std::setprecision(1) << std::setw(8) << fantasyPoints << std::endl;
    }
    
    // Display detailed player statistics
    void displayDetailedStats() const {
        std::cout << "\n===== " << name << " (" << positionToString(position) << ", " << team << ") =====\n";
        std::cout << "Games Played: " << gamesPlayed << std::endl;
        std::cout << "Fantasy Points: " << std::fixed << std::setprecision(1) << fantasyPoints << std::endl;
        
        switch (position) {
            case Position::QUARTERBACK:
                std::cout << "Passing Yards: " << passingYards << std::endl;
                std::cout << "Passing TDs: " << passingTouchdowns << std::endl;
                std::cout << "Interceptions: " << interceptions << std::endl;
                std::cout << "Rushing Yards: " << rushingYards << std::endl;
                std::cout << "Rushing TDs: " << rushingTouchdowns << std::endl;
                break;
            case Position::RUNNING_BACK:
                std::cout << "Rushing Yards: " << rushingYards << std::endl;
                std::cout << "Rushing TDs: " << rushingTouchdowns << std::endl;
                std::cout << "Receiving Yards: " << receivingYards << std::endl;
                std::cout << "Receiving TDs: " << receivingTouchdowns << std::endl;
                break;
            case Position::WIDE_RECEIVER:
            case Position::TIGHT_END:
                std::cout << "Receiving Yards: " << receivingYards << std::endl;
                std::cout << "Receiving TDs: " << receivingTouchdowns << std::endl;
                break;
            case Position::KICKER:
                std::cout << "Field Goals: " << fieldGoals << std::endl;
                std::cout << "Extra Points: " << extraPoints << std::endl;
                break;
            case Position::DEFENSE:
                std::cout << "Sacks: " << sacks << std::endl;
                std::cout << "Interceptions: " << defensiveInterceptions << std::endl;
                std::cout << "Defensive TDs: " << defensiveTouchdowns << std::endl;
                break;
        }
    }
};

// Team class to store team information and roster
class Team {
private:
    std::string name;
    std::string owner;
    std::vector<Player*> roster;
    std::vector<Player*> activeLineup;
    int wins;
    int losses;
    double totalPoints;

public:
    Team(const std::string& name, const std::string& owner)
        : name(name), owner(owner), wins(0), losses(0), totalPoints(0.0) {}
    
    // Getters
    std::string getName() const { return name; }
    std::string getOwner() const { return owner; }
    int getWins() const { return wins; }
    int getLosses() const { return losses; }
    double getTotalPoints() const { return totalPoints; }
    
    // Add a player to the roster
    bool addPlayer(Player* player) {
        // Check if player is already on the roster
        for (const auto& p : roster) {
            if (p->getId() == player->getId()) {
                return false;
            }
        }
        roster.push_back(player);
        return true;
    }
    
    // Remove a player from the roster
    bool removePlayer(int playerId) {
        for (auto it = roster.begin(); it != roster.end(); ++it) {
            if ((*it)->getId() == playerId) {
                // Also remove from active lineup if present
                for (auto lit = activeLineup.begin(); lit != activeLineup.end(); ++lit) {
                    if ((*lit)->getId() == playerId) {
                        activeLineup.erase(lit);
                        break;
                    }
                }
                roster.erase(it);
                return true;
            }
        }
        return false;
    }
    
    // Display team roster
    void displayRoster() const {
        Utility::displayTitle("TEAM ROSTER: " + name + " (Owner: " + owner + ")");
        std::cout << std::left << std::setw(4) << "ID" << " | "
                  << std::setw(20) << "Name" << " | "
                  << std::setw(5) << "Pos" << " | "
                  << std::setw(15) << "Team" << " | "
                  << std::setw(8) << "Points" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        
        for (const auto& player : roster) {
            player->displayInfo();
        }
        
        std::cout << "\nTeam Record: " << wins << "-" << losses << std::endl;
        std::cout << "Total Points: " << std::fixed << std::setprecision(1) << totalPoints << std::endl;
    }
    
    // Set active lineup
    bool setLineup(const std::vector<int>& playerIds) {
        activeLineup.clear();
        
        for (int id : playerIds) {
            bool found = false;
            for (auto& player : roster) {
                if (player->getId() == id) {
                    activeLineup.push_back(player);
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false; // Player ID not found in roster
            }
        }
        
        return true;
    }
    
    // Display active lineup
    void displayLineup() const {
        Utility::displayTitle("ACTIVE LINEUP: " + name + " (Owner: " + owner + ")");
        
        if (activeLineup.empty()) {
            std::cout << "No active lineup set.\n";
            return;
        }
        
        std::cout << std::left << std::setw(4) << "ID" << " | "
                  << std::setw(20) << "Name" << " | "
                  << std::setw(5) << "Pos" << " | "
                  << std::setw(15) << "Team" << " | "
                  << std::setw(8) << "Points" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        
        for (const auto& player : activeLineup) {
            player->displayInfo();
        }
    }
    
    // Simulate a game performance for all players in active lineup
    double simulateGame() {
        double gameScore = 0.0;
        
        for (auto& player : activeLineup) {
            player->simulateGamePerformance();
            gameScore += player->getFantasyPoints();
        }
        
        // Add the game score to total points
        totalPoints += gameScore;
        
        return gameScore;
    }
    
    // Update team record
    void updateRecord(bool isWin) {
        if (isWin) {
            wins++;
        } else {
            losses++;
        }
    }
    
    // Get roster size
    int getRosterSize() const {
        return roster.size();
    }
    
    // Get lineup size
    int getLineupSize() const {
        return activeLineup.size();
    }
    
    // Get player by ID from the roster
    Player* getPlayerById(int id) {
        for (auto& player : roster) {
            if (player->getId() == id) {
                return player;
            }
        }
        return nullptr;
    }
    
    // Get all players
    const std::vector<Player*>& getRoster() const {
        return roster;
    }
    
    // Get active lineup
    const std::vector<Player*>& getActiveLineup() const {
        return activeLineup;
    }
};

// Matchup class to represent a game between two teams
class Matchup {
private:
    Team* homeTeam;
    Team* awayTeam;
    double homeScore;
    double awayScore;
    bool isCompleted;

public:
    Matchup(Team* home, Team* away)
        : homeTeam(home), awayTeam(away), homeScore(0.0), awayScore(0.0), isCompleted(false) {}
    
    // Simulate the matchup
    void simulate() {
        if (homeTeam->getLineupSize() == 0 || awayTeam->getLineupSize() == 0) {
            std::cout << "Warning: One or both teams don't have an active lineup. Skipping matchup.\n";
            return;
        }
        
        homeScore = homeTeam->simulateGame();
        awayScore = awayTeam->simulateGame();
        
        // Update team records
        if (homeScore > awayScore) {
            homeTeam->updateRecord(true);
            awayTeam->updateRecord(false);
        } else if (awayScore > homeScore) {
            homeTeam->updateRecord(false);
            awayTeam->updateRecord(true);
        } else {
            // In case of a tie, give both teams a loss (to keep it simple)
            homeTeam->updateRecord(false);
            awayTeam->updateRecord(false);
        }
        
        isCompleted = true;
    }
    
    // Display matchup results
    void displayResults() const {
        std::cout << "\n==== MATCHUP RESULTS ====\n";
        std::cout << homeTeam->getName() << " vs " << awayTeam->getName() << std::endl;
        
        if (isCompleted) {
            std::cout << "Score: " << std::fixed << std::setprecision(1) 
                      << homeScore << " - " << awayScore << std::endl;
            
            if (homeScore > awayScore) {
                std::cout << homeTeam->getName() << " wins!\n";
            } else if (awayScore > homeScore) {
                std::cout << awayTeam->getName() << " wins!\n";
            } else {
                std::cout << "It's a tie!\n";
            }
        } else {
            std::cout << "Matchup not yet simulated.\n";
        }
    }
    
    // Getters
    Team* getHomeTeam() const { return homeTeam; }
    Team* getAwayTeam() const { return awayTeam; }
    double getHomeScore() const { return homeScore; }
    double getAwayScore() const { return awayScore; }
    bool getIsCompleted() const { return isCompleted; }
};

// League class to manage teams, players, and matchups
class League {
private:
    std::string name;
    std::vector<Team*> teams;
    std::vector<Player*> availablePlayers;
    std::vector<Matchup*> schedule;
    int currentWeek;
    int maxTeams;
    int rosterSize;
    int lineupSize;

public:
    League(const std::string& name, int maxTeams, int rosterSize, int lineupSize)
        : name(name), currentWeek(1), maxTeams(maxTeams), rosterSize(rosterSize), lineupSize(lineupSize) {
        // Initialize available players with hard-coded data
        initializePlayers();
    }
    
    ~League() {
        // Cleanup memory
        for (auto& team : teams) {
            delete team;
        }
        for (auto& player : availablePlayers) {
            delete player;
        }
        for (auto& matchup : schedule) {
            delete matchup;
        }
    }
    
    // Initialize players with hard-coded data
    void initializePlayers() {
        // Quarterbacks
        availablePlayers.push_back(new Player(1, "Patrick Mahomes", Position::QUARTERBACK, "Kansas City"));
        availablePlayers.push_back(new Player(2, "Josh Allen", Position::QUARTERBACK, "Buffalo"));
        availablePlayers.push_back(new Player(3, "Lamar Jackson", Position::QUARTERBACK, "Baltimore"));
        availablePlayers.push_back(new Player(4, "Joe Burrow", Position::QUARTERBACK, "Cincinnati"));
        availablePlayers.push_back(new Player(5, "Justin Herbert", Position::QUARTERBACK, "Los Angeles"));
        availablePlayers.push_back(new Player(6, "Jalen Hurts", Position::QUARTERBACK, "Philadelphia"));
        
        // Running Backs
        availablePlayers.push_back(new Player(7, "Christian McCaffrey", Position::RUNNING_BACK, "San Francisco"));
        availablePlayers.push_back(new Player(8, "Derrick Henry", Position::RUNNING_BACK, "Tennessee"));
        availablePlayers.push_back(new Player(9, "Jonathan Taylor", Position::RUNNING_BACK, "Indianapolis"));
        availablePlayers.push_back(new Player(10, "Nick Chubb", Position::RUNNING_BACK, "Cleveland"));
        availablePlayers.push_back(new Player(11, "Saquon Barkley", Position::RUNNING_BACK, "New York"));
        availablePlayers.push_back(new Player(12, "Austin Ekeler", Position::RUNNING_BACK, "Los Angeles"));
        availablePlayers.push_back(new Player(13, "Alvin Kamara", Position::RUNNING_BACK, "New Orleans"));
        availablePlayers.push_back(new Player(14, "Dalvin Cook", Position::RUNNING_BACK, "Minnesota"));
        
        // Wide Receivers
        availablePlayers.push_back(new Player(15, "Justin Jefferson", Position::WIDE_RECEIVER, "Minnesota"));
        availablePlayers.push_back(new Player(16, "Tyreek Hill", Position::WIDE_RECEIVER, "Miami"));
        availablePlayers.push_back(new Player(17, "Davante Adams", Position::WIDE_RECEIVER, "Las Vegas"));
        availablePlayers.push_back(new Player(18, "Cooper Kupp", Position::WIDE_RECEIVER, "Los Angeles"));
        availablePlayers.push_back(new Player(19, "Stefon Diggs", Position::WIDE_RECEIVER, "Buffalo"));
        availablePlayers.push_back(new Player(20, "CeeDee Lamb", Position::WIDE_RECEIVER, "Dallas"));
        availablePlayers.push_back(new Player(21, "A.J. Brown", Position::WIDE_RECEIVER, "Philadelphia"));
        availablePlayers.push_back(new Player(22, "Ja'Marr Chase", Position::WIDE_RECEIVER, "Cincinnati"));
        availablePlayers.push_back(new Player(23, "Deebo Samuel", Position::WIDE_RECEIVER, "San Francisco"));
        availablePlayers.push_back(new Player(24, "Mike Evans", Position::WIDE_RECEIVER, "Tampa Bay"));
        
        // Tight Ends
        availablePlayers.push_back(new Player(25, "Travis Kelce", Position::TIGHT_END, "Kansas City"));
        availablePlayers.push_back(new Player(26, "Mark Andrews", Position::TIGHT_END, "Baltimore"));
        availablePlayers.push_back(new Player(27, "George Kittle", Position::TIGHT_END, "San Francisco"));
        availablePlayers.push_back(new Player(28, "T.J. Hockenson", Position::TIGHT_END, "Detroit"));
        availablePlayers.push_back(new Player(29, "Dallas Goedert", Position::TIGHT_END, "Philadelphia"));
        
        // Kickers
        availablePlayers.push_back(new Player(30, "Justin Tucker", Position::KICKER, "Baltimore"));
        availablePlayers.push_back(new Player(31, "Harrison Butker", Position::KICKER, "Kansas City"));
        availablePlayers.push_back(new Player(32, "Evan McPherson", Position::KICKER, "Cincinnati"));
        availablePlayers.push_back(new Player(33, "Tyler Bass", Position::KICKER, "Buffalo"));
        
        // Defenses
        availablePlayers.push_back(new Player(34, "San Francisco 49ers", Position::DEFENSE, "San Francisco"));
        availablePlayers.push_back(new Player(35, "Dallas Cowboys", Position::DEFENSE, "Dallas"));
        availablePlayers.push_back(new Player(36, "Buffalo Bills", Position::DEFENSE, "Buffalo"));
        availablePlayers.push_back(new Player(37, "New England Patriots", Position::DEFENSE, "New England"));
        availablePlayers.push_back(new Player(38, "Pittsburgh Steelers", Position::DEFENSE, "Pittsburgh"));
    }
    
    // Register a new team
    bool registerTeam(const std::string& teamName, const std::string& ownerName) {
        if (teams.size() >= maxTeams) {
            std::cout << "Maximum number of teams reached.\n";
            return false;
        }
        
        // Check if team name is already taken
        for (const auto& team : teams) {
            if (team->getName() == teamName) {
                std::cout << "Team name already exists. Please choose another name.\n";
                return false;
            }
        }
        
        teams.push_back(new Team(teamName, ownerName));
        std::cout << "Team \"" << teamName << "\" successfully registered!\n";
        return true;
    }
    
    // Display all teams
    void displayTeams() const {
        Utility::displayTitle("TEAMS");
        
        if (teams.empty()) {
            std::cout << "No teams registered yet.\n";
            return;
        }
        
        std::cout << std::left << std::setw(5) << "No." << " | "
                  << std::setw(20) << "Team" << " | "
                  << std::setw(20) << "Owner" << " | "
                  << std::setw(10) << "Record" << " | "
                  << std::setw(10) << "Points" << std::endl;
        std::cout << std::string(70, '-') << std::endl;
        
        for (size_t i = 0; i < teams.size(); ++i) {
            std::cout << std::left << std::setw(5) << (i + 1) << " | "
                      << std::setw(20) << teams[i]->getName() << " | "
                      << std::setw(20) << teams[i]->getOwner() << " | "
                      << std::setw(10) << teams[i]->getWins() << "-" << teams[i]->getLosses() << " | "
                      << std::fixed << std::setprecision(1) << std::setw(10) << teams[i]->getTotalPoints() << std::endl;
        }
    }
    
    // Display available players
    void displayAvailablePlayers() const {
        Utility::displayTitle("AVAILABLE PLAYERS");
        
        if (availablePlayers.empty()) {
            std::cout << "No available players.\n";
            return;
        }
        
        std::cout << std::left << std::setw(4) << "ID" << " | "
                  << std::setw(20) << "Name" << " | "
                  << std::setw(5) << "Pos" << " | "
                  << std::setw(15) << "Team" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        
        for (const auto& player : availablePlayers) {
            std::cout << std::left << std::setw(4) << player->getId() << " | "
                      << std::setw(20) << player->getName() << " | "
                      << std::setw(5) << positionToString(player->getPosition()) << " | "
                      << std::setw(15) << player->getTeam() << std::endl;
        }
    }
    
    // Display players by position
    void displayPlayersByPosition(Position pos) const {
        std::string posStr = positionToString(pos);
        Utility::displayTitle("AVAILABLE " + posStr + "s");
        
        bool foundPlayers = false;
        
        std::cout << std::left << std::setw(4) << "ID" << " | "
                  << std::setw(20) << "Name" << " | "
                  << std::setw(15) << "Team" << std::endl;
        std::cout << std::string(45, '-') << std::endl;
        
        for (const auto& player : availablePlayers) {
            if (player->getPosition() == pos) {
                std::cout << std::left << std::setw(4) << player->getId() << " | "
                          << std::setw(20) << player->getName() << " | "
                          << std::setw(15) << player->getTeam() << std::endl;
                foundPlayers = true;
            }
        }
        
        if (!foundPlayers) {
            std::cout << "No available players at position " << posStr << ".\n";
        }
    }
    
    // Draft a player to a team
    bool draftPlayer(Team* team, int playerId) {
        // Find the player in available players
        auto it = std::find_if(availablePlayers.begin(), availablePlayers.end(),
                              [playerId](const Player* p) { return p->getId() == playerId; });
        
        if (it == availablePlayers.end()) {
            std::cout << "Player ID " << playerId << " not found in available players.\n";
            return false;
        }
        
        if (team->getRosterSize() >= rosterSize) {
            std::cout << "Team roster is full (" << rosterSize << " players maximum).\n";
            return false;
        }
        
        Player* player = *it;
        
        // Add player to team
        if (team->addPlayer(player)) {
            // Remove from available players
            availablePlayers.erase(it);
            std::cout << player->getName() << " has been drafted to " << team->getName() << "!\n";
            return true;
        } else {
            std::cout << "Player is already on the team roster.\n";
            return false;
        }
    }
    
    // Set team lineup
    bool setTeamLineup(Team* team, const std::vector<int>& playerIds) {
        if (playerIds.size() != lineupSize) {
            std::cout << "Lineup must have exactly " << lineupSize << " players.\n";
            return false;
        }
        
        if (team->setLineup(playerIds)) {
            std::cout << "Lineup for " << team->getName() << " has been set.\n";
            return true;
        } else {
            std::cout << "Failed to set lineup. Please check player IDs.\n";
            return false;
        }
    }
    
    // Generate matchups for the current week
    void generateMatchups() {
        // Clear previous matchups
        for (auto& matchup : schedule) {
            delete matchup;
        }
        schedule.clear();
        
        // We need at least 2 teams for matchups
        if (teams.size() < 2) {
            std::cout << "Need at least 2 teams to generate matchups.\n";
            return;
        }
        
        // Create a copy of team pointers that we can shuffle
        std::vector<Team*> shuffledTeams = teams;
        
        // Shuffle teams randomly
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(shuffledTeams.begin(), shuffledTeams.end(), g);
        
        // Create matchups
        for (size_t i = 0; i < shuffledTeams.size(); i += 2) {
            // If we have an odd number of teams, the last team gets a bye
            if (i + 1 >= shuffledTeams.size()) {
                std::cout << shuffledTeams[i]->getName() << " has a bye this week.\n";
                continue;
            }
            
            Matchup* matchup = new Matchup(shuffledTeams[i], shuffledTeams[i + 1]);
            schedule.push_back(matchup);
            
            std::cout << "Matchup: " << shuffledTeams[i]->getName() 
                     << " vs " << shuffledTeams[i + 1]->getName() << std::endl;
        }
    }
    
    // Simulate all matchups for the current week
    void simulateWeek() {
        if (schedule.empty()) {
            std::cout << "No matchups scheduled. Generate matchups first.\n";
            return;
        }
        
        std::cout << "\n==== SIMULATING WEEK " << currentWeek << " ====\n";
        
        for (auto& matchup : schedule) {
            matchup->simulate();
            matchup->displayResults();
        }
        
        // Advance to next week
        currentWeek++;
    }
    
    // Display league standings
    void displayStandings() const {
        Utility::displayTitle("LEAGUE STANDINGS");
        
        if (teams.empty()) {
            std::cout << "No teams registered yet.\n";
            return;
        }
        
        // Create a copy of teams that we can sort
        std::vector<Team*> sortedTeams = teams;
        
        // Sort teams by wins (descending), then by points (descending)
        std::sort(sortedTeams.begin(), sortedTeams.end(), [](const Team* a, const Team* b) {
            if (a->getWins() != b->getWins()) {
                return a->getWins() > b->getWins();
            }
            return a->getTotalPoints() > b->getTotalPoints();
        });
        
        std::cout << std::left << std::setw(5) << "Rank" << " | "
                  << std::setw(20) << "Team" << " | "
                  << std::setw(20) << "Owner" << " | "
                  << std::setw(10) << "Record" << " | "
                  << std::setw(10) << "Points" << std::endl;
        std::cout << std::string(70, '-') << std::endl;
        
        for (size_t i = 0; i < sortedTeams.size(); ++i) {
            std::cout << std::left << std::setw(5) << (i + 1) << " | "
                      << std::setw(20) << sortedTeams[i]->getName() << " | "
                      << std::setw(20) << sortedTeams[i]->getOwner() << " | "
                      << std::setw(5) << sortedTeams[i]->getWins() << "-" << std::setw(4) << sortedTeams[i]->getLosses() << " | "
                      << std::fixed << std::setprecision(1) << std::setw(10) << sortedTeams[i]->getTotalPoints() << std::endl;
        }
    }
    
    // Display all player statistics
    void displayPlayerStatistics() const {
        Utility::displayTitle("PLAYER STATISTICS");
        
        // Create a vector of all players (from teams and available)
        std::vector<Player*> allPlayers;
        
        // Add players from teams
        for (const auto& team : teams) {
            const auto& roster = team->getRoster();
            allPlayers.insert(allPlayers.end(), roster.begin(), roster.end());
        }
        
        // Add available players
        allPlayers.insert(allPlayers.end(), availablePlayers.begin(), availablePlayers.end());
        
        // Sort players by fantasy points (descending)
        std::sort(allPlayers.begin(), allPlayers.end(), [](const Player* a, const Player* b) {
            return a->getFantasyPoints() > b->getFantasyPoints();
        });
        
        // Display table header
        std::cout << std::left << std::setw(4) << "ID" << " | "
                  << std::setw(20) << "Name" << " | "
                  << std::setw(5) << "Pos" << " | "
                  << std::setw(15) << "Team" << " | "
                  << std::setw(8) << "Points" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        
        // Display players
        for (const auto& player : allPlayers) {
            if (player->getFantasyPoints() > 0) { // Only show players with points
                player->displayInfo();
            }
        }
    }
    
    // Display weekly report
    void weeklyReport() const {
        Utility::displayTitle("WEEK " + std::to_string(currentWeek - 1) + " REPORT");
        
        if (schedule.empty()) {
            std::cout << "No matchups have been simulated yet.\n";
            return;
        }
        
        // Show matchup results
        std::cout << "==== MATCHUP RESULTS ====\n";
        for (const auto& matchup : schedule) {
            if (matchup->getIsCompleted()) {
                std::cout << matchup->getHomeTeam()->getName() << " vs " << matchup->getAwayTeam()->getName();
                std::cout << ": " << std::fixed << std::setprecision(1) 
                          << matchup->getHomeScore() << " - " << matchup->getAwayScore() << std::endl;
            }
        }
        
        // Show team standings
        displayStandings();
        
        // Show top 5 players of the week
        std::cout << "\n==== TOP PERFORMERS ====\n";
        
        // Create a vector of all players from teams
        std::vector<Player*> allPlayers;
        for (const auto& team : teams) {
            const auto& roster = team->getRoster();
            allPlayers.insert(allPlayers.end(), roster.begin(), roster.end());
        }
        
        // Sort players by fantasy points (descending)
        std::sort(allPlayers.begin(), allPlayers.end(), [](const Player* a, const Player* b) {
            return a->getFantasyPoints() > b->getFantasyPoints();
        });
        
        // Display table header
        std::cout << std::left << std::setw(4) << "ID" << " | "
                  << std::setw(20) << "Name" << " | "
                  << std::setw(5) << "Pos" << " | "
                  << std::setw(15) << "Team" << " | "
                  << std::setw(8) << "Points" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        
        // Display top 5 players (or fewer if there aren't that many)
        int count = 0;
        for (const auto& player : allPlayers) {
            if (player->getFantasyPoints() > 0) { // Only show players with points
                player->displayInfo();
                count++;
                if (count >= 5) break;
            }
        }
    }
    
    // Getters
    int getCurrentWeek() const { return currentWeek; }
    int getTeamCount() const { return teams.size(); }
    int getMaxTeams() const { return maxTeams; }
    int getRosterSize() const { return rosterSize; }
    int getLineupSize() const { return lineupSize; }
    
    // Get team by index
    Team* getTeam(int index) {
        if (index >= 0 && index < static_cast<int>(teams.size())) {
            return teams[index];
        }
        return nullptr;
    }
    
    // Get available player by ID
    Player* getAvailablePlayerById(int id) {
        for (auto& player : availablePlayers) {
            if (player->getId() == id) {
                return player;
            }
        }
        return nullptr;
    }
    
    // Get player by ID from all players (teams and available)
    Player* getPlayerById(int id) {
        // First check available players
        for (auto& player : availablePlayers) {
            if (player->getId() == id) {
                return player;
            }
        }
        
        // Then check team rosters
        for (auto& team : teams) {
            Player* player = team->getPlayerById(id);
            if (player) {
                return player;
            }
        }
        
        return nullptr;
    }
    
    // Check if all teams have full rosters
    bool allTeamsHaveFullRosters() const {
        for (const auto& team : teams) {
            if (team->getRosterSize() < rosterSize) {
                return false;
            }
        }
        return !teams.empty(); // Only return true if there are teams and they all have full rosters
    }
};

// Main application class
class SportsManagerApp {
private:
    League* league;
    
    // Display main menu
    void displayMainMenu() const {
        Utility::clearScreen();
        Utility::displayTitle("SPORTS TEAM MANAGER");
        std::cout << "WEEK " << league->getCurrentWeek() << std::endl;
        std::cout << "1. Register Team\n";
        std::cout << "2. View Teams\n";
        std::cout << "3. Draft Players\n";
        std::cout << "4. Set Lineup\n";
        std::cout << "5. View Available Players\n";
        std::cout << "6. View Player Details\n";
        std::cout << "7. Generate Matchups\n";
        std::cout << "8. Simulate Week\n";
        std::cout << "9. View Standings\n";
        std::cout << "10. View Player Statistics\n";
        std::cout << "11. Weekly Report\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter choice: ";
    }
    
    // Register a new team
    void registerTeamMenu() {
        Utility::clearScreen();
        Utility::displayTitle("REGISTER NEW TEAM");
        
        if (league->getTeamCount() >= league->getMaxTeams()) {
            std::cout << "Maximum number of teams (" << league->getMaxTeams() << ") reached.\n";
            std::cout << "Press Enter to continue...";
            std::cin.get();
            return;
        }
        
        std::cout << "Enter team name: ";
        std::string teamName = Utility::getStringInput();
        
        std::cout << "Enter owner name: ";
        std::string ownerName = Utility::getStringInput();
        
        league->registerTeam(teamName, ownerName);
        
        std::cout << "Press Enter to continue...";
        std::cin.get();
    }
    
    // View teams menu
    void viewTeamsMenu() {
        Utility::clearScreen();
        league->displayTeams();
        
        std::cout << "\nOptions:\n";
        std::cout << "1. View Team Roster\n";
        std::cout << "2. View Team Lineup\n";
        std::cout << "0. Back to Main Menu\n";
        std::cout << "Enter choice: ";
        
        int choice = Utility::getIntInput(0, 2);
        
        if (choice == 0) {
            return;
        }
        
        if (league->getTeamCount() == 0) {
            std::cout << "No teams registered yet.\n";
            std::cout << "Press Enter to continue...";
            std::cin.get();
            return;
        }
        
        std::cout << "Enter team number: ";
        int teamIndex = Utility::getIntInput(1, league->getTeamCount()) - 1;
        
        Team* team = league->getTeam(teamIndex);
        if (!team) {
            std::cout << "Invalid team selection.\n";
            std::cout << "Press Enter to continue...";
            std::cin.get();
            return;
        }
        
        Utility::clearScreen();
        if (choice == 1) {
            team->displayRoster();
        } else if (choice == 2) {
            team->displayLineup();
        }
        
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }
    
    // Draft players menu
    void draftPlayersMenu() {
        Utility::clearScreen();
        Utility::displayTitle("DRAFT PLAYERS");
        
        if (league->getTeamCount() == 0) {
            std::cout << "No teams registered yet.\n";
            std::cout << "Press Enter to continue...";
            std::cin.get();
            return;
        }
        
        if (league->allTeamsHaveFullRosters()) {
            std::cout << "All teams have full rosters.\n";
            std::cout << "Press Enter to continue...";
            std::cin.get();
            return;
        }
        
        // Select team
        league->displayTeams();
        std::cout << "\nSelect team to draft for (enter team number): ";
        int teamIndex = Utility::getIntInput(1, league->getTeamCount()) - 1;
        
        Team* team = league->getTeam(teamIndex);
        if (!team) {
            std::cout << "Invalid team selection.\n";
            std::cout << "Press Enter to continue...";
            std::cin.get();
            return;
        }
        
        if (team->getRosterSize() >= league->getRosterSize()) {
            std::cout << team->getName() << " already has a full roster.\n";
            std::cout << "Press Enter to continue...";
            std::cin.get();
            return;
        }
        
        Utility::clearScreen();
        std::cout << "Drafting for: " << team->getName() << " (" << team->getRosterSize() << "/" << league->getRosterSize() << " players)\n\n";
        
        // Display available players by position
        std::cout << "Select position to view:\n";
        std::cout << "1. Quarterbacks (QB)\n";
        std::cout << "2. Running Backs (RB)\n";
        std::cout << "3. Wide Receivers (WR)\n";
        std::cout << "4. Tight Ends (TE)\n";
        std::cout << "5. Kickers (K)\n";
        std::cout << "6. Defenses (DEF)\n";
        std::cout << "7. All Available Players\n";
        std::cout << "Enter choice: ";
        
        int posChoice = Utility::getIntInput(1, 7);
        
        Utility::clearScreen();
        if (posChoice == 7) {
            league->displayAvailablePlayers();
        } else {
            Position position;
            switch (posChoice) {
                case 1: position = Position::QUARTERBACK; break;
                case 2: position = Position::RUNNING_BACK; break;
                case 3: position = Position::WIDE_RECEIVER; break;
                case 4: position = Position::TIGHT_END; break;
                case 5: position = Position::KICKER; break;
                case 6: position = Position::DEFENSE; break;
                default: position = Position::QUARTERBACK; break;
            }
            league->displayPlayersByPosition(position);
        }
        
        std::cout << "\nEnter Player ID to draft (0 to cancel): ";
        int playerId = Utility::getIntInput(0, 100);
        
        if (playerId == 0) {
            return;
        }
        
        Player* player = league->getAvailablePlayerById(playerId);
        if (!player) {
            std::cout << "Invalid player ID or player already drafted.\n";
            std::cout << "Press Enter to continue...";
            std::cin.get();
            return;
        }
        
        league->draftPlayer(team, playerId);
        
        std::cout << "Press Enter to continue...";
        std::cin.get();
        
        // Loop back to this menu if roster isn't full
        if (team->getRosterSize() < league->getRosterSize()) {
            draftPlayersMenu();
        }
    }
    
    // Set lineup menu
    void setLineupMenu() {
        Utility::clearScreen();
        Utility::displayTitle("SET LINEUP");
        
        if (league->getTeamCount() == 0) {
            std::cout << "No teams registered yet.\n";
            std::cout << "Press Enter to continue...";
            std::cin.get();
            return;
        }
        
        // Select team
        league->displayTeams();
        std::cout << "\nSelect team (enter team number): ";
        int teamIndex = Utility::getIntInput(1, league->getTeamCount()) - 1;
        
        Team* team = league->getTeam(teamIndex);
        if (!team) {
            std::cout << "Invalid team selection.\n";
            std::cout << "Press Enter to continue...";
            std::cin.get();
            return;
        }
        
        if (team->getRosterSize() == 0) {
            std::cout << team->getName() << " has no players. Draft players first.\n";
            std::cout << "Press Enter to continue...";
            std::cin.get();
            return;
        }
        
        if (team->getRosterSize() < league->getLineupSize()) {
            std::cout << team->getName() << " does not have enough players for a lineup (" 
                     << league->getLineupSize() << " needed, " << team->getRosterSize() << " on roster).\n";
            std::cout << "Press Enter to continue...";
            std::cin.get();
            return;
        }
        
        Utility::clearScreen();
        std::cout << "Setting lineup for: " << team->getName() << "\n\n";
        
        // Display roster
        std::cout << "Team roster:\n";
        team->displayRoster();
        
        std::vector<int> lineupIds;
        std::cout << "\nSelect " << league->getLineupSize() << " players for your lineup:\n";
        
        for (int i = 0; i < league->getLineupSize(); ++i) {
            std::cout << "Enter Player #" << (i + 1) << " ID: ";
            int playerId = Utility::getIntInput(1, 100);
            
            Player* player = team->getPlayerById(playerId);
            if (!player) {
                std::cout << "Player ID " << playerId << " not found on this team's roster. Try again.\n";
                --i; // Retry this selection
                continue;
            }
            
            // Check if player is already in lineup
            if (std::find(lineupIds.begin(), lineupIds.end(), playerId) != lineupIds.end()) {
                std::cout << "Player is already in your lineup. Pick a different player.\n";
                --i; // Retry this selection
                continue;
            }
            
            lineupIds.push_back(playerId);
        }
        
        league->setTeamLineup(team, lineupIds);
        
        std::cout << "Press Enter to continue...";
        std::cin.get();
    }
    
    // View available players menu
    void viewAvailablePlayersMenu() {
        Utility::clearScreen();
        
        std::cout << "Select position to view:\n";
        std::cout << "1. Quarterbacks (QB)\n";
        std::cout << "2. Running Backs (RB)\n";
        std::cout << "3. Wide Receivers (WR)\n";
        std::cout << "4. Tight Ends (TE)\n";
        std::cout << "5. Kickers (K)\n";
        std::cout << "6. Defenses (DEF)\n";
        std::cout << "7. All Available Players\n";
        std::cout << "Enter choice: ";
        
        int posChoice = Utility::getIntInput(1, 7);
        
        Utility::clearScreen();
        if (posChoice == 7) {
            league->displayAvailablePlayers();
        } else {
            Position position;
            switch (posChoice) {
                case 1: position = Position::QUARTERBACK; break;
                case 2: position = Position::RUNNING_BACK; break;
                case 3: position = Position::WIDE_RECEIVER; break;
                case 4: position = Position::TIGHT_END; break;
                case 5: position = Position::KICKER; break;
                case 6: position = Position::DEFENSE; break;
                default: position = Position::QUARTERBACK; break;
            }
            league->displayPlayersByPosition(position);
        }
        
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }
    
    // View player details menu
    void viewPlayerDetailsMenu() {
        Utility::clearScreen();
        Utility::displayTitle("PLAYER DETAILS");
        
        std::cout << "Enter Player ID (1-38): ";
        int playerId = Utility::getIntInput(1, 38);
        
        Player* player = league->getPlayerById(playerId);
        if (!player) {
            std::cout << "Player ID " << playerId << " not found.\n";
            std::cout << "Press Enter to continue...";
            std::cin.get();
            return;
        }
        
        player->displayDetailedStats();
        
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }
    
    // Generate matchups menu
    void generateMatchupsMenu() {
        Utility::clearScreen();
        Utility::displayTitle("GENERATE MATCHUPS");
        
        if (league->getTeamCount() < 2) {
            std::cout << "Need at least 2 teams to generate matchups.\n";
            std::cout << "Press Enter to continue...";
            std::cin.get();
            return;
        }
        
        // Check if all teams have active lineups
        bool allTeamsReady = true;
        for (int i = 0; i < league->getTeamCount(); ++i) {
            Team* team = league->getTeam(i);
            if (team->getLineupSize() == 0) {
                std::cout << team->getName() << " does not have an active lineup yet.\n";
                allTeamsReady = false;
            }
        }
        
        if (!allTeamsReady) {
            std::cout << "\nWarning: Some teams do not have active lineups. They will not score points during simulation.\n";
            std::cout << "Do you want to continue anyway? (1 for Yes, 0 for No): ";
            int choice = Utility::getIntInput(0, 1);
            if (choice == 0) {
                return;
            }
        }
        
        std::cout << "\nGenerating matchups for Week " << league->getCurrentWeek() << ":\n";
        league->generateMatchups();
        
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }
    
    // Simulate week menu
    void simulateWeekMenu() {
        Utility::clearScreen();
        Utility::displayTitle("SIMULATE WEEK " + std::to_string(league->getCurrentWeek()));
        
        league->simulateWeek();
        
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }
    
    // View standings menu
    void viewStandingsMenu() {
        Utility::clearScreen();
        league->displayStandings();
        
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }
    
    // View player statistics menu
    void viewPlayerStatisticsMenu() {
        Utility::clearScreen();
        league->displayPlayerStatistics();
        
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }
    
    // Weekly report menu
    void weeklyReportMenu() {
        Utility::clearScreen();
        league->weeklyReport();
        
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }

public:
    SportsManagerApp() {
        // Create a league with 8 max teams, 10 roster spots, 5 lineup spots
        league = new League("Fantasy Football League", 8, 10, 5);
    }
    
    ~SportsManagerApp() {
        delete league;
    }
    
    void run() {
        bool running = true;
        
        while (running) {
            displayMainMenu();
            int choice = Utility::getIntInput(0, 11);
            
            switch (choice) {
                case 0: running = false; break;
                case 1: registerTeamMenu(); break;
                case 2: viewTeamsMenu(); break;
                case 3: draftPlayersMenu(); break;
                case 4: setLineupMenu(); break;
                case 5: viewAvailablePlayersMenu(); break;
                case 6: viewPlayerDetailsMenu(); break;
                case 7: generateMatchupsMenu(); break;
                case 8: simulateWeekMenu(); break;
                case 9: viewStandingsMenu(); break;
                case 10: viewPlayerStatisticsMenu(); break;
                case 11: weeklyReportMenu(); break;
            }
        }
        
        std::cout << "Thank you for using Sports Team Manager. Goodbye!\n";
    }
};

int main() {
    SportsManagerApp app;
    app.run();
    return 0;
}