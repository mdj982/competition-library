#include "auto_util_header.hpp"

class Nim {
private:
	bool result;
public:
	Nim(vll a) {
		ll x = 0;
		Loop(i, a.size()) x ^= a[i];
		if (x != 0) result = true;
		else result = false;
	}
	bool get_result() {
		return result;
	}
	string get_winner(string player1, string player2) {
		if (result) return player1;
		else return player2;
	}
};

class Grundy {
private:
	vi grundies;
	vi(*transition_rule)(int);
	int get_grundy_val(int state) {
		// memoization
		if (grundies[state] == -1) {
			set<int> s;
			vi next_states = transition_rule(state);
			for (const int next_state : next_states) {
				s.insert(this->get_grundy_val(next_state));
			}
			int c = 0;
			while (s.find(c) != s.end()) c++;
			grundies[state] = c;
		}
		return grundies[state];
	}
public:
	Grundy(vi(*transition_rule)(int), int STATE_SIZE) {
		// TODO: allocate code of states
		this->grundies = vi(STATE_SIZE, -1);
		this->transition_rule = transition_rule;
	}
	bool solve_game(const vi &states) {
		// decide the grundy number in each states
		vll xs(states.size());
		Loop(i, states.size()) {
			xs[i] = this->get_grundy_val(states[i]);
		}
		// return to Nim
	    Nim *nim = new Nim(xs);
    	bool result = nim->get_result();
		delete nim;
		return result;
	}
	string solve_game(const vi &states, string player0, string player1){
		if (this->solve_game(states)) {
			return player0;
		}
		else {
			return player1;
		}
	}
};

vi transition_rule(int state) {
	vi next_states;
	{
		// TODO: transition_rule
		if (state > 0) {
			next_states.push_back(state - 1);
		}
	}
	return next_states;
}
