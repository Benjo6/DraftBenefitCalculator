#ifndef DRAFTAPI_H
#define DRAFTAPI_H

#include <cpprest/http_listener.h>
#include <cpprest/json.h>

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

// A class that represents the web API for the draft benefit % calculator
class DraftApi {
public:
	// Constructor that takes the URI of the web API
	DraftApi(utility::string_t url);

	// Destructor
	~DraftApi();

	// Method to initialize the Web API
	pplx::task<void> open();

	// Method to shut down the Web API
	pplx::task<void> close();

private:
	// Method to handle GET requests
	void handle_get(http_request message);

	// Method to handle POST requests
	void handle_post(http_request message);

	// Method to handle PUT requests
	void handle_put(http_request message);

	// Method to handle DELETE requests
	void handle_delete(http_request message);

	// Method to calculate the draft benefit % from the request parameters
	double calculate_draft_benefit(json::value request);

	// Method to calculate the air density from the temperature, pressure, and humidity
	double calculate_air_density(double temperature, double pressure, double humidity);

	// Method to calculate the drag force from the air density, frontal area, drag coefficient, and relative velocity
	double calculate_drag_force(double air_density, double frontal_area, double drag_coefficient, double relative_velocity);	

	// Method to calculate the power output from the speed, gradient, drag force, and mass
	double calculate_power_output(double speed, double gradient, double drag_force, double mass);

	// The http listener that handles requests and responses
	http_listener m_listener;
};
#endif // !DRAFTAPI_H
