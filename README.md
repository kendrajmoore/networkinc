# Network Latency Analyzer

## Overview

The **Network Latency Analyzer** is a simple C-based tool that measures and analyzes network latency by sending ICMP Echo Request ("ping") packets and calculating the time it takes to receive ICMP Echo Replies. This tool is useful for understanding the performance and reliability of a network connection.

## Features

- **Measure Network Latency**: Calculates the round-trip time (RTT) for a packet to travel from the local machine to a specified remote host.
- **Customizable Target**: Users can specify the IP address of the target host to measure latency.
- **Timeout Handling**: Detects and reports timeouts when the target host is unreachable or doesn't respond within the specified time.
- **Default IP Address**: If no target IP is provided, the tool defaults to pinging `8.8.8.8` (Google's DNS server).

## Installation

1. Clone the repository:
    ```bash
    git clone https://github.com/kendramoore/networkinc.git
    ```

2. Navigate to the project directory:
    ```bash
    cd networkinc
    ```

3. Compile the program using `gcc`:
    ```bash
    gcc -o networkinc networkinc.c
    ```

## Usage

### Running the Program

You need to run the program with root privileges due to the use of raw sockets. You can provide a target IP address or use the default IP (`8.8.8.8`).

```bash
sudo ./networkinc <IP_ADDRESS>
```

### Examples

- **Pinging a Specific Host:**
    ```bash
    sudo ./networkinc 192.168.1.1
    ```

- **Using the Default Target (`8.8.8.8`):**
    ```bash
    sudo ./networkinc
    ```

### Output

The tool outputs the round-trip time (RTT) in milliseconds. If the request times out, a message will indicate that the target did not respond within the timeout period.

```
Reply from 8.8.8.8: time=20.34 ms
```

## How It Works

- The tool uses **raw sockets** to send ICMP Echo Requests (similar to `ping`).
- **ICMP Headers** are manually crafted and checked for correctness using a checksum function.
- It measures the time between sending an Echo Request and receiving an Echo Reply to calculate network latency.
- The tool includes **basic error handling** for timeouts and unreachable hosts.

## Troubleshooting

- **Request Timed Out**: If you see a "Request timed out" message, it could be due to:
  - The target machine not responding to ICMP requests (check firewall settings).
  - Network issues preventing communication.
  - The target IP being unreachable.

- **Permission Denied**: Ensure you are running the program with `sudo` as root privileges are required for raw socket operations.

## Contributing

Contributions are welcome! Feel free to open issues or submit pull requests to improve the tool or add new features.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

This README provides an introduction to the Network Latency Analyzer tool, usage instructions, and troubleshooting tips. You can adjust it as needed based on your specific implementation.
