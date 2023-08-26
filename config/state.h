using std::string;

class tas_vars {
public:
	int gamestate;
	int zone_progress;

	string gamestate_name() {
		switch (gamestate) {
			case 5:
				return "Normandy with Nihilus";
				break;
			case 10:
				return "Eden Prime";
				break;
			case 20:
				return "Presidium 1";
				break;
			default:
				return "State Unknown - possible error. ";
				break;
		}
	}
};