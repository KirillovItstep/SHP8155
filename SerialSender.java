//Отправляет данные CAN пакета в последовательный порт
public class SerialSender {
    public static void main(String[] args) throws InterruptedException {
        ComPort.open("COM7", 115200);
        Thread.sleep(50); //После открытия обязательно подождать!!!
        ComPort.port.flushIOBuffers();

        // Отправляем данные
        long id = 0x18FE40F9; // 4 байта
        byte[] data = {0, 0, 0, 0, (byte) 0xFD, 0, 0, 0}; // 8 байт
        int delay = 100; // 2 байта

        // Преобразуем в массив байтов
        byte[] dataToSend = new byte[14];
        System.arraycopy(longToBytes(id), 0, dataToSend, 0, 4);
        System.arraycopy(data, 0, dataToSend, 4, 8);
        System.arraycopy(intToBytes(delay), 0, dataToSend, 12, 2);

        // Отправляем данные
        ComPort.write(dataToSend);
        Thread.sleep(150);
        System.out.println(ComPort.readLine());
        Thread.sleep(50);
        Thread.sleep(2500);
        ComPort.close();
    }

    private static byte[] longToBytes(long value) {
        return new byte[]{
                (byte) (value & 0xFF),
                (byte) ((value >> 8) & 0xFF),
                (byte) ((value >> 16) & 0xFF),
                (byte) ((value >> 24) & 0xFF)
        };
    }

    private static byte[] intToBytes(int value) {
        return new byte[]{
                (byte) (value & 0xFF),
                (byte) ((value >> 8) & 0xFF)
        };
    }
}
