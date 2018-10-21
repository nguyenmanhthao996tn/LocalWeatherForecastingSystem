import java.util.Date;

import org.bson.Document;

public class SensorDataObject {
	/* Define masks */
	private static int NodeId15_8_MASK = 0xFF;
	private static int NodeId7_0_MASK = 0xFF;
	private static int Time7_0_MASK = 0xFF;
	private static int Time10_8_MASK = 0xE0;
	private static int Battery_MASK = 0x1F;
	private static int AirDirection_MASK = 0xE0;
	private static int AirSpeed1Min9_5_MASK = 0x1F;
	private static int AirSpeed1Min4_0_MASK = 0xF8;
	private static int Temperature2_0_MASK = 0x07;
	private static int Temperature6_3_MASK = 0xF0;
	private static int AirSpeed5Min3_0_MASK = 0x0F;
	private static int AirSpeed5Min9_4_MASK = 0xFC;
	private static int Rainfall1H9_8_MASK = 0x03;
	private static int Rainfall1H7_0_MASK = 0xFF;
	private static int Rainfall24H9_2_MASK = 0xFF;
	private static int Rainfall24H1_0_MASK = 0xC0;
	private static int NodeKey6_1_MASK = 0x3F;
	private static int Humidity_MASK = 0xFE;
	private static int NodeKey0_MASK = 0x01;
	private static int Atmosphere15_8_MASK = 0xFF;
	private static int Atmosphere7_0_MASK = 0xFF;
	private static int CRC_MASK = 0xFF;

	/* Define positions */
	private static int NodeId15_8_POSITION = 0;
	private static int NodeId7_0_POSITION = 1;
	private static int Time7_0_POSITION = 2;
	private static int Time10_8_POSITION = 3;
	private static int Battery_POSITION = 3;
	private static int AirDirection_POSITION = 4;
	private static int AirSpeed1Min9_5_POSITION = 4;
	private static int AirSpeed1Min4_0_POSITION = 5;
	private static int Temperature2_0_POSITION = 5;
	private static int Temperature6_3_POSITION = 6;
	private static int AirSpeed5Min3_0_POSITION = 6;
	private static int AirSpeed5Min9_4_POSITION = 7;
	private static int Rainfall1H9_8_POSITION = 7;
	private static int Rainfall1H7_0_POSITION = 8;
	private static int Rainfall24H9_2_POSITION = 9;
	private static int Rainfall24H1_0_POSITION = 10;
	private static int NodeKey6_1_POSITION = 10;
	private static int Humidity_POSITION = 11;
	private static int NodeKey0_POSITION = 11;
	private static int Atmosphere15_8_POSITION = 12;
	private static int Atmosphere7_0_POSITION = 13;
	private static int CRC_POSITION = 14;

	/* Define offset */
	private static int NodeId15_8_OFFSET = 0;
	private static int NodeId7_0_OFFSET = 0;
	private static int Time7_0_OFFSET = 0;
	private static int Time10_8_OFFSET = 5;
	private static int Battery_OFFSET = 0;
	private static int AirDirection_OFFSET = 5;
	private static int AirSpeed1Min9_5_OFFSET = 0;
	private static int AirSpeed1Min4_0_OFFSET = 3;
	private static int Temperature2_0_OFFSET = 0;
	private static int Temperature6_3_OFFSET = 4;
	private static int AirSpeed5Min3_0_OFFSET = 0;
	private static int AirSpeed5Min9_4_OFFSET = 2;
	private static int Rainfall1H9_8_OFFSET = 0;
	private static int Rainfall1H7_0_OFFSET = 0;
	private static int Rainfall24H9_2_OFFSET = 0;
	private static int Rainfall24H1_0_OFFSET = 6;
	private static int NodeKey6_1_OFFSET = 0;
	private static int Humidity_OFFSET = 1;
	private static int NodeKey0_OFFSET = 0;
	private static int Atmosphere15_8_OFFSET = 0;
	private static int Atmosphere7_0_OFFSET = 0;
	private static int CRC_OFFSET = 0;

	String dataString;
	int nodeId;
	int nodeKey;
	int battery;
	int airDirection;
	int airSpeed1Min;
	int airSpeed5Min;
	int temperature;
	int rainfall1Hour;
	int rainfall24Hour;
	int humidity;
	int atmosphere;
	Date time;

	public SensorDataObject(String input) throws Exception {
		super();

		this.dataString = input;

		this.time = new Date(); // get current time

		int[] inputByteArray = new int[input.length()];
		String[] bytes = input.split(",");

		for (int i = 0; i < bytes.length; i++) {
			inputByteArray[i] = Integer.parseInt(bytes[i]);
		}

		int crc = (inputByteArray[CRC_POSITION] & CRC_MASK) >> CRC_OFFSET;
		int calculatedCrc = Crc8(inputByteArray, bytes.length - 1);
		if (crc != calculatedCrc) {
			// CRC ERROR
			throw (new Exception("CRC Error | Calculated: " + calculatedCrc + " | Read: " + crc));
		} else {
			this.nodeId = ((inputByteArray[NodeId15_8_POSITION] & NodeId15_8_MASK) >> NodeId15_8_OFFSET) << 8;
			this.nodeId |= (inputByteArray[NodeId7_0_POSITION] & NodeId7_0_MASK) >> NodeId7_0_OFFSET;

			// this.time = ((inputByteArray[Time10_8_POSITION] & Time10_8_MASK) >>
			// Time10_8_OFFSET) << 8;
			// this.time |= (inputByteArray[Time7_0_POSITION] & Time7_0_MASK) >>
			// Time7_0_OFFSET;

			this.battery = (inputByteArray[Battery_POSITION] & Battery_MASK) >> Battery_OFFSET;

			this.airDirection = (inputByteArray[AirDirection_POSITION] & AirDirection_MASK) >> AirDirection_OFFSET;

			this.airSpeed1Min = ((inputByteArray[AirSpeed1Min9_5_POSITION]
					& AirSpeed1Min9_5_MASK) >> AirSpeed1Min9_5_OFFSET) << 5;
			this.airSpeed1Min |= (inputByteArray[AirSpeed1Min4_0_POSITION]
					& AirSpeed1Min4_0_MASK) >> AirSpeed1Min4_0_OFFSET;

			this.airSpeed5Min = ((inputByteArray[AirSpeed5Min9_4_POSITION]
					& AirSpeed5Min9_4_MASK) >> AirSpeed5Min9_4_OFFSET) << 4;
			this.airSpeed5Min |= (inputByteArray[AirSpeed5Min3_0_POSITION]
					& AirSpeed5Min3_0_MASK) >> AirSpeed5Min3_0_OFFSET;

			this.temperature = ((inputByteArray[Temperature6_3_POSITION]
					& Temperature6_3_MASK) >> Temperature6_3_OFFSET) << 3;
			this.temperature |= (inputByteArray[Temperature2_0_POSITION]
					& Temperature2_0_MASK) >> Temperature2_0_OFFSET;

			this.rainfall1Hour = ((inputByteArray[Rainfall1H9_8_POSITION]
					& Rainfall1H9_8_MASK) >> Rainfall1H9_8_OFFSET) << 8;
			this.rainfall1Hour |= (inputByteArray[Rainfall1H7_0_POSITION] & Rainfall1H7_0_MASK) >> Rainfall1H7_0_OFFSET;

			this.rainfall24Hour = ((inputByteArray[Rainfall24H9_2_POSITION]
					& Rainfall24H9_2_MASK) >> Rainfall24H9_2_OFFSET) << 2;
			this.rainfall24Hour |= (inputByteArray[Rainfall24H1_0_POSITION]
					& Rainfall24H1_0_MASK) >> Rainfall24H1_0_OFFSET;

			this.nodeKey = ((inputByteArray[NodeKey6_1_POSITION] & NodeKey6_1_MASK) >> NodeKey6_1_OFFSET) << 1;
			this.nodeKey |= (inputByteArray[NodeKey0_POSITION] & NodeKey0_MASK) >> NodeKey0_OFFSET;

			this.humidity = (inputByteArray[Humidity_POSITION] & Humidity_MASK) >> Humidity_OFFSET;

			this.atmosphere = ((inputByteArray[Atmosphere15_8_POSITION]
					& Atmosphere15_8_MASK) >> Atmosphere15_8_OFFSET) << 8;
			this.atmosphere |= (inputByteArray[Atmosphere7_0_POSITION] & Atmosphere7_0_MASK) >> Atmosphere7_0_OFFSET;
		}
	}

	public Document toDocument() {
		Document resultDocument = new Document();

		resultDocument.append("Time", this.time);
		resultDocument.append("AirDirection", this.airDirection * 45);
		resultDocument.append("AirSpeed1Min", this.airSpeed1Min);
		resultDocument.append("AirSpeed5Min", this.airSpeed5Min);
		resultDocument.append("Temperature", this.temperature);
		resultDocument.append("Rainfall1Hour", this.rainfall1Hour);
		resultDocument.append("Rainfall24Hour", this.rainfall24Hour);
		resultDocument.append("Humidity", this.humidity);
		resultDocument.append("Atmosphere", this.atmosphere);
		resultDocument.append("Battery", this.battery * 5);

		return resultDocument;
	}

	int Crc8(int[] vptr, int len) {
		int crc = 0, dataIndex = 0;
		int data = vptr[dataIndex];
		int i, j;
		for (j = len; j > 0; j--, dataIndex++) {
			data = vptr[dataIndex];
			crc ^= (data << 8);
			for (i = 8; i > 0; i--) {
				if ((crc & 0x8000) != 0)
					crc ^= (0x1070 << 3);
				crc <<= 1;
			}
		}
		return (crc >> 8) & 0xff;
	}
}
