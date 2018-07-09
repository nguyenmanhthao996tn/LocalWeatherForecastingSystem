import java.net.*;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.io.*;

public class Main {
	private static final int NUMBER_OF_BACKGROUND_WORKER = 5;
	static List<Thread> backgroundWorkerThreadList;
	static Queue<Workload> workloadQueue;
	static int workloadListCounter = 0;
	static RequestHandler requestHandler;
	private static final Object lock = new Object();

	public static class WorkerThread extends Thread {
		private int id;

		public WorkerThread(int id) {
			super();
			this.id = id;
		}

		@Override
		public void run() {
			int counter;
			Workload workload = null;

			super.run();

			while (true) {
				synchronized (lock) {
					counter = workloadListCounter;
					if (counter > 0) {
						workloadListCounter--;
						workload = workloadQueue.remove();
					}
				}

				if ((counter > 0) && (workload != null)) {
					try {
						// Process request
						Socket socket = workload.getSocketServer();
						String request = workload.getRequest();
						DataOutputStream out = new DataOutputStream(socket.getOutputStream());
						System.out.println("[REQUEST] ip: " + socket.getRemoteSocketAddress() + " request: " + request);
						out.write(requestHandler.processRequest(request).getBytes());

						workload.getSocketServer().close();
					} catch (SocketException e) {
						System.out.println("[EXCEPTION] " + id + " Connection reset");
					} catch (IOException e) {
						System.out.println("[EXCEPTION] " + id + " " + e.getMessage());
					}
				}
			}
		}
	}

	public static void main(String[] args) {
		// Init
		requestHandler = new RequestHandler();
		workloadQueue = new LinkedList<Workload>();
		backgroundWorkerThreadList = new ArrayList<>();
		for (int i = 0; i < NUMBER_OF_BACKGROUND_WORKER; i++) {
			backgroundWorkerThreadList.add(new WorkerThread(i));
			backgroundWorkerThreadList.get(i).start();
		}

		// Main thread
		try {
			ServerSocket serverSocket = new ServerSocket(5001);
			serverSocket.setSoTimeout(10000);

			while (true) {
				try {
					Socket socketServer = serverSocket.accept();
					DataInputStream in = new DataInputStream(socketServer.getInputStream());
					byte[] bufferArray = new byte[255];
					in.read(bufferArray);
					String request = new String(bufferArray);
					Workload newWorkload = new Workload(request, socketServer);

					synchronized (lock) {
						workloadQueue.add(newWorkload);
						workloadListCounter++;
					}

				} catch (SocketTimeoutException e) {
					System.out.println("[EXCEPTION] " + e.getMessage());
				} catch (Exception e) {
					System.out.println("[EXCEPTION] " + e.getMessage());
					serverSocket.close();
					break;
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
