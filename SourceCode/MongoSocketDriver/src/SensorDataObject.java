import java.util.Date;

import org.bson.Document;

public class SensorDataObject {
	String dataString;
	int airDirection;
	int airSpeed1Min;
	int airSpeed5Min;
	int temperature;
	int rainfall1Hour;
	int rainfall24Hour;
	int humidity;
	int atmosphere;
	Date time;

	public SensorDataObject(String input) {
		super();

		this.dataString = input;

		time = new Date(); // get current time

		String[] arr = input.split("c|s|g|t|r|p|h|b");

		try {
			this.airDirection = Integer.parseInt(arr[1]);
		} catch (NumberFormatException e) {
			this.airDirection = 0;
		}
		try {
			this.airSpeed1Min = Integer.parseInt(arr[2]);
		} catch (NumberFormatException e) {
			this.airSpeed1Min = 0;
		}
		try {
			this.airSpeed5Min = Integer.parseInt(arr[3]);
		} catch (NumberFormatException e) {
			this.airSpeed5Min = 0;
		}
		try {
			this.temperature = Integer.parseInt(arr[4]);
		} catch (NumberFormatException e) {
			this.temperature = 0;
		}
		try {
			this.rainfall1Hour = Integer.parseInt(arr[5]);
		} catch (NumberFormatException e) {
			this.rainfall1Hour = 0;
		}
		try {
			this.rainfall24Hour = Integer.parseInt(arr[6]);
		} catch (NumberFormatException e) {
			this.rainfall24Hour = 0;
		}
		try {
			this.humidity = Integer.parseInt(arr[7]);
		} catch (NumberFormatException e) {
			this.humidity = 0;
		}
		try {
			this.atmosphere = Integer.parseInt(arr[8]);
		} catch (NumberFormatException e) {
			this.atmosphere = 0;
		}
	}
	
	public Document toDocument() {
		Document resultDocument = new Document();
		
		resultDocument.append("Time", this.time);
		resultDocument.append("AirDirection", this.airDirection);
		resultDocument.append("AirSpeed1Min", this.airSpeed1Min);
		resultDocument.append("AirSpeed5Min", this.airSpeed5Min);
		resultDocument.append("Temperature", this.temperature);
		resultDocument.append("Rainfall1Hour", this.rainfall1Hour);
		resultDocument.append("Rainfall24Hour", this.rainfall24Hour);
		resultDocument.append("Humidity", this.humidity);
		resultDocument.append("Atmosphere", this.atmosphere);
		
		return resultDocument;
	}

	public String getDataString() {
		return dataString;
	}

	public int getAirDirection() {
		return airDirection;
	}

	public int getAirSpeed1Min() {
		return airSpeed1Min;
	}

	public int getAirSpeed5Min() {
		return airSpeed5Min;
	}

	public int getTemperature() {
		return temperature;
	}

	public int getRainfall1Hour() {
		return rainfall1Hour;
	}

	public int getRainfall24Hour() {
		return rainfall24Hour;
	}

	public int getHumidity() {
		return humidity;
	}

	public int getAtmosphere() {
		return atmosphere;
	}

	public Date getTime() {
		return time;
	}

}
