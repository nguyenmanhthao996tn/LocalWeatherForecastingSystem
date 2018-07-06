import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.TimeZone;

import org.bson.Document;
import org.bson.json.JsonParseException;

import com.mongodb.ConnectionString;
import com.mongodb.client.MongoClient;
import com.mongodb.client.MongoClients;
import com.mongodb.client.MongoCollection;
import com.mongodb.client.MongoCursor;
import com.mongodb.client.MongoDatabase;

public class RequestHandler {
	public static final int REQUEST_CODE_READ_SENSOR_DATA = 0;
	public static final int REQUEST_CODE_WRITE_SENSOR_DATA = 1;
	public static final int REQUEST_CODE_WRITE_FORECAST_RESULT = 2;

	public static final String MONGODB_URI = "mongodb://localhost:27017";
	public static final String DATABASE_NAME = "WeatherForecast";
	public static final String SENSOR_DATA_COLLECTION_NAME = "WeatherData";
	public static final String FORECAST_RESULT_COLLECTION_NAME = "ForecastResult";

	ConnectionString connectionString;
	MongoClient mongoClient;
	MongoDatabase database;
	MongoCollection<Document> sensorDataCollection, forecastResultCollection;
	SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm");

	public RequestHandler() {
		// Initialize MongoDB
		connectionString = new ConnectionString(MONGODB_URI);
		mongoClient = MongoClients.create(connectionString);
		database = mongoClient.getDatabase(DATABASE_NAME);
		sensorDataCollection = database.getCollection(SENSOR_DATA_COLLECTION_NAME);
		forecastResultCollection = database.getCollection(FORECAST_RESULT_COLLECTION_NAME);
	}

	public String processRequest(String request) {
		String requestResult = "";

		// Process request
		try {
			Document d = Document.parse(request);
			int code = d.getInteger("code");
			switch (code) {
			case REQUEST_CODE_READ_SENSOR_DATA:
				String fromDateString = d.getString("from");
				Date fromDate = formatter.parse(fromDateString);
				String toDateString = d.getString("to");
				Date toDate = formatter.parse(toDateString);
				requestResult = readSensorData(fromDate, toDate);
				break;
			case REQUEST_CODE_WRITE_SENSOR_DATA:
				String dataString = d.getString("data");
				SensorDataObject dataObject = new SensorDataObject(dataString);
				insertSensorData(dataObject.toDocument());
				requestResult = "{\"status\":\"OK\"}";
				break;
			case REQUEST_CODE_WRITE_FORECAST_RESULT:
				String forecastDateString = d.getString("date");
				Date forecastDate = formatter.parse(forecastDateString);
				boolean isRain = d.getBoolean("rain");
				int amountOfRain = d.getInteger("AmountOfRain");
				insertForecastResult(new Document("Date", forecastDate).append("IsRain", isRain).append("AmountOfRain", amountOfRain));
				requestResult = "{\"status\":\"OK\"}";
				break;
			default:
				break;
			}
		} catch (ParseException e) {
			System.out.println("[ParseException] " + e.getMessage());
			requestResult = "{\"code\":\"FAIL\"}";
		} catch (JsonParseException e) {
			System.out.println("[JsonParseException] " + e.getMessage());
			requestResult = "{\"code\":\"FAIL\"}";
		} catch (Exception e) {
			System.out.println("[EXCEPTION] " + e.getMessage());
			requestResult = "{\"code\":\"FAIL\"}";
		}

		return requestResult;
	}

	public void close() {
		mongoClient.close();
	}

	private void insertSensorData(Document objectToInsert) {
		// Add information to data object

		// Insert
		sensorDataCollection.insertOne(objectToInsert);
	}

	private String readSensorData(Date from, Date to) {
		String result = "";

		Document querryObject = new Document("Time", new Document().append("$gte", from).append("$lte", to));
		MongoCursor<Document> cursor = sensorDataCollection.find(querryObject).iterator();
		try {
			while (cursor.hasNext()) {
				result += sensorDataDocumentToString(cursor.next()) + "\r\n";
			}
		} finally {
			cursor.close();
		}

		return result;
	}

	private String sensorDataDocumentToString(Document document) {
		String result = "";

		Date time = document.getDate("Time");
		TimeZone tz = TimeZone.getTimeZone("UTC");
		DateFormat df = new SimpleDateFormat("yyyy-MM-dd'T'HH:mm'Z'"); // Quoted "Z" to indicate UTC, no timezone offset
		df.setTimeZone(tz);
		String timeAsISO = df.format(time);

		int airDirection = document.getInteger("AirDirection");
		int airSpeed1Min = document.getInteger("AirSpeed1Min");
		int airSpeed5Min = document.getInteger("AirSpeed5Min");
		int temperature = document.getInteger("Temperature");
		int rainfall1Hour = document.getInteger("Rainfall1Hour");
		int rainfall24Hour = document.getInteger("Rainfall24Hour");
		int humidity = document.getInteger("Humidity");
		int atmosphere = document.getInteger("Atmosphere");

		result += timeAsISO + " " + airDirection + " " + airSpeed1Min + " " + airSpeed5Min + " " + temperature + " "
				+ rainfall1Hour + " " + rainfall24Hour + " " + humidity + " " + atmosphere;

		return result;
	}

	private void insertForecastResult(Document objectToInsert) {
		// Add information to data object

		// Insert
		forecastResultCollection.insertOne(objectToInsert);
	}
}
