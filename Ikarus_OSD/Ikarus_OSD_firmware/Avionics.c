/* 
 * (c) 2010 Rafael Paz <rpaz@atc.us.es>
 *
 *************
 *
 *	This file is part of IKARUS_OSD.
 *
 *  IKARUS_OSD is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  IKARUS_OSD is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with IKARUS_OSD.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <math.h>
 
#define air_gas_constant 287.052f			// 8.31432f
#define air_density_sea_level 1.225f
#define absolute_null_kelvin 273.15f

// Pressure at sea level in pascal
#define pressure_sea_level 101325.0f
 

float calc_altitude_pressure(float pressure_ambient)
{
	return 44330.0f * ((1-pow((pressure_ambient/pressure_sea_level),(1/5.255f)))); // Meters
}
 
float get_air_density(float static_pressure, float temperature_celsius)
{
  return static_pressure/(air_gas_constant * (temperature_celsius + absolute_null_kelvin));
}
 
/**
 * @brief Calculate indicated airspeed
 *
 * Please note that the indicated airspeed is not the true airspeed because it
 * lacks the air density compensation. Use the calc_true_airspeed functions to get
 * the true airspeed.
 *
 * @parem pressure_front pressure inside the pitot/prandl tube
 * @param pressure_ambient pressure at the side of the tube/airplane
 * @return indicated airspeed in m/s
 */
float calc_indicated_airspeed(float pressure_front, float pressure_ambient)
{
  return sqrt((2.0f*(pressure_front - pressure_ambient))/air_density_sea_level);
}
 
/**
 * @brief Calculate true airspeed from indicated airspeed
 *
 * Please note that the true airspeed is NOT the groundspeed, because of the effects of wind
 *
 * @parem speed current indicated airspeed
 * @param pressure_ambient pressure at the side of the tube/airplane
 * @param temperature air temperature in degrees celcius
 * @return true airspeed in m/s
 */
float calc_true_airspeed_from_indicated(float speed, float pressure_ambient, float temperature)
{
  return speed * sqrt(air_density_sea_level/get_air_density(pressure_ambient, temperature));
}
 
/**
 * @brief Directly calculate true airspeed
 *
 * Please note that the true airspeed is NOT the groundspeed, because of the effects of wind
 *
 * @parem pressure_front pressure inside the pitot/prandl tube
 * @param pressure_ambient pressure at the side of the tube/airplane
 * @param temperature air temperature in degrees celcius
 * @return true airspeed in m/s
 */
float calc_true_airspeed(float pressure_front, float pressure_ambient, float temperature)
{
  return sqrt((2.0f*(pressure_front - pressure_ambient))/get_air_density(pressure_ambient, temperature));
}