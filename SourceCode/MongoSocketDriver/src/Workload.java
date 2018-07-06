import java.net.Socket;

public class Workload {
	private String request;
	private Socket socketServer;

	// public Workload() {
	// super();
	// this.request = "";
	// this.socketServer = null;
	// }

	public Workload(String request, Socket socketServer) {
		super();
		this.request = request;
		this.socketServer = socketServer;
	}

	public String getRequest() {
		return request;
	}

	public void setRequest(String request) {
		this.request = request;
	}

	public Socket getSocketServer() {
		return socketServer;
	}

	public void setSocketServer(Socket socketServer) {
		this.socketServer = socketServer;
	}

	@Override
	public String toString() {
		return "Workload [request=" + request + ", socketServer=" + socketServer + "]";
	}
}