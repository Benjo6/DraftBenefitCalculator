#include "DraftAPI.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <cmath>

// Constructor that takes the URI of the Web API
DraftApi::DraftApi(utility::string_t url) : m_listener(url)
{
	// Register the methods to handle the requests
	m_listener.support(methods::GET, std::bind(&DraftApi::handle_get, this, std::placeholders::_1));
	m_listener.support(methods::POST, std::bind(&DraftApi::handle_post, this, std::placeholders::_1));
	m_listener.support(methods::PUT, std::bind(&DraftApi::handle_put, this, std::placeholders::_1));
	m_listener.support(methods::DEL, std::bind(&DraftApi::handle_delete, this, std::placeholders::_1));
}

// Destructor
DraftApi::~DraftApi()
{
}

// Method to initialize the Web API
pplx::task<void> DraftApi::open() {
	return m_listener.open();
}

// Method to shut down the Web API
pplx::task<void> DraftApi::close() {
	return m_listener.close(); 
}

// Method to handle the GET requests
void DraftApi::handle_get(http_request message) {
	// Send a response with a message that the GET method is not supported
	message.reply(status_codes::BadRequest,
		U("GET method is not supported. Please use POST method to send the parameters."));
}

// Method to handle the POST requests
void DraftApi::handle_post(http_request message) {
	// Try to parse the JSON object from the request body
	try
	{
		// Get the JSON object from the request body
		json::value request = message.extract_json().get();

		// Calculate the draft benefit % from the request parameters
		double result = calculate_draft_benefit(request);

		// Send a response with the draft benefit % as a JSON value
		message.reply(status_codes::OK, json::value::number(result));
	}
	catch (const std::exception&)
	{
		// Send a response with an error message
		message.reply(status_codes::BadRequest, U("Invalid JSON object. Please check the parameters and the format."));
	}
}

// Method to handle the PUT requests
void DraftApi::handle_put(http_request message) 
{	// Send a response with a message that the PUT method is not supported{
	message.reply(status_codes::BadRequest,
		U("PUT method is not supported. Please use POST method to send the parameters."));
}

// Method to handle the DELETE requests
void DraftApi::handle_delete(http_request message)
{
	// Send a response with a message that the DELETE method is not supported
	message.reply(status_codes::BadRequest, U("DELETE method is not supported. Please use POST method to send the parameters."));
}

// Method to calculate the draft benefit % from the request parameters
double DraftApi::calculate_draft_benefit(json::value request) {
	// Get the parameters from the request object
	double speed = request[U("Speed")].as_double(); // the speed of the cyclist in km/h
	double distance = request[U("Distance")].as_double(); // the drafting distance in m
	double gradient = request[U("Gradient")].as_double(); // the road gradient in %
	double wind = request[U("Wind")].as_double(); // the wind speed in km/h
	double angle = request[U("Angle")].as_double(); // the wind angle in degrees
	double temp = request[U("Temp")].as_double(); // the temperature of the environment in °C
	double pressure = request[U("Pressure")].as_double();  // the pressure of the environment in kPa
	double humidity = request[U("Humidity")].as_double(); // the humidity of the environment in %
	double area = request[U("Area")].as_double(); // the frontal area of the cyclist in m^2
	double drag = request[U("Drag")].as_double(); // the drag coefficient of the cyclist

	// Convert the units to SI units
	speed = speed / 3.6; // convert the speed from km/h to m/s
	wind = wind / 3.6; // convert the wind speed from km/h to m/s
	gradient = gradient / 100; // convert the gradient from % to decimal
	pressure = pressure * 100; // convert the pressure from kPa to Pa
	angle = angle * M_PI / 180; // degrees to radians

	// Define some constants
	double mass = 80; // the mass of the cyclist and the bike in kg
	double g = 9.81; // the gravitational acceleration in m/s^2

	// Calculate the air density from the temperature, pressure, and humidity
	double air_density = calculate_air_density(temp, pressure, humidity);

	// Calculate the relative velocity of the cyclist and the wind
	double relative_velocity = speed - wind * cos(angle);

	// Calculate the drag force on the cyclist
	double drag_force = calculate_drag_force(air_density, area, drag, relative_velocity);

	// Calculate the power output of the cyclist without drafting
	double power_without_drafting = calculate_power_output(speed, gradient, drag_force, mass);

	// Calculate the drag force on the cyclist with drafting
	double drag_force_with_drafting = drag_force * (1 - exp(-distance / 0.175));

	// Calculate the power output of the cyclist with drafting
	double power_with_drafting = calculate_power_output(speed, gradient, drag_force_with_drafting, mass);

	// Calculate the draft benefit % as the percentage of power saved by drafting
	double draft_benefit = (power_without_drafting - power_with_drafting) / power_without_drafting * 100;

	// Return the draft benefit %
	return draft_benefit;
}

// Method to calculate the air density from the temperature, pressure, and humidity
double DraftApi::calculate_air_density(double temp, double pressure, double humidity) {
	// Define some constants
	double R = 287.058; // the specific gas constant for dry air in J/(kg*K)
	double Rd = 461.495; // the specific gas constant for water vapor in J/(kg*K)
	double L = 2.5e6; // the latent heat of vaporization of water in J/kg
	double T0 = 273.15;  // the reference temperature in K
	double e0 = 6.11; // the reference vapor pressure in hPa
	double c = 0.11; // the enhancement factor

	// Convert the temperature to Kelvin
	temp = temp + T0;

	// Calculate the saturation vapor pressure
	double es = e0 * exp(L / Rd * (1 / T0 - 1 / temp));

	// Calculate the actual vapor pressure
	double e = humidity / 100 * es;

	// Calculate the air density
	double air_destiny = (pressure-e * (1-c))/ (R * temp) + e * (1-c) / (Rd * temp);

	// Return the air density
	return air_destiny;
}

// Method to calculate the drag force from the air density, frontal area, drag coefficient, and relative velocity
double DraftApi::calculate_drag_force(double air_density, double area, double drag, double relative_velocity) {
	
	// Calculate the drag force
	double drag_force = 0.5 * air_density * area * drag * pow(relative_velocity, 2);
	// Return the drag force
	return drag_force;
}

// Method to calculate the power output from the speed, gradient, drag force, and mass double
double DraftApi::calculate_power_output(double speed, double gradient, double drag_force, double mass) 
{
	// Define some constants
	double g = 9.81; // the gravitational acceleration in m/s^2
	double crr = 0.005; // the coefficient of rolling resistance

	// Calculate the power output
	double power_output = speed * (drag_force + mass * g * (sin(atan(gradient)) + crr * cos(atan(gradient))));
	
	// Return the power output
	return power_output;
}

// The main function that runs the program 
int main() {
	// Define the URI of the web API
	utility::string_t address = U("http://localhost:8080/api/draft");


	// Create an instance of the DraftApi class
	DraftApi api(address);

	// Initialize the Web API
	api.open().wait();

	// Wait for use input to close the Web API
	std::cout << "Press ENTER to exit." <<
		std::endl;
	std::string line;
	std::getline(std::cin, line);


	// Close the Web API
	api.close().wait();

	// Return 0 to indicate success
	return 0;
}