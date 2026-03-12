const vscode = require('vscode');

class AeroLangHoverProvider {
    constructor() {
        // Detailed Markdown definitions for Hovering over keywords
        this.docs = {
            'LocationManager': new vscode.MarkdownString(
                '**AeroLang Hardware API: LocationManager**\n\n' +
                'Polls device GPS and Network location statuses natively.\n' +
                '```aerolang\nval loc = LocationManager()\nval lat = loc.getLatitude()\n```'
            ),
            'SensorManager': new vscode.MarkdownString(
                '**AeroLang Hardware API: SensorManager**\n\n' +
                'Interfaces directly with the device Accelerometer, Gyroscope, Magnetometer, and Proximity hardware sensors.\n' +
                '```aerolang\nval sensors = SensorManager()\nsensors.startListening()\n```'
            ),
            'VideoView': new vscode.MarkdownString(
                '**AeroLang UI Component: VideoView**\n\n' +
                'A visual component capable of asynchronously decoding and playing local or remote video feeds.\n' +
                '```aerolang\nval video = VideoView()\nvideo.setVideoPath("file.mp4")\nvideo.start()\n```'
            ),
            'String': new vscode.MarkdownString(
                '**AeroLang Standard Type: String**\n\n' +
                'A unified ASCII/UTF-8 Native C++ string wrapper mapping directly to `std::string` backend.'
            ),
            'Int': new vscode.MarkdownString(
                '**AeroLang Standard Type: Int**\n\n' +
                'A 32-bit signed integer.'
            ),
            'Double': new vscode.MarkdownString(
                '**AeroLang Standard Type: Double**\n\n' +
                'A double-precision 64-bit IEEE 754 floating point.'
            ),
            'Float': new vscode.MarkdownString(
                '**AeroLang Standard Type: Float**\n\n' +
                'A single-precision 32-bit IEEE 754 floating point.'
            ),
            'Boolean': new vscode.MarkdownString(
                '**AeroLang Standard Type: Boolean**\n\n' +
                'A true or false value.'
            ),
            'Unit': new vscode.MarkdownString(
                '**AeroLang Standard Type: Unit**\n\n' +
                'The type with only one value: the Unit object. Corresponds to `void`.'
            ),
            'Console': new vscode.MarkdownString(
                '**AeroLang Utility: Console**\n\n' +
                'Standard JS-style logging and timing interactions.\n' +
                '```aerolang\nConsole.log("Connecting...")\n```'
            ),
            'Log': new vscode.MarkdownString(
                '**AeroLang Utility: Log**\n\n' +
                'Android Logcat system bindings for `d`, `i`, `e`, `w` log levels.\n' +
                '```aerolang\nLog.i("Tag", "Message")\n```'
            ),
            'Timer': new vscode.MarkdownString(
                '**AeroLang Utility: Timer**\n\n' +
                'Asynchronous timed executions.\n' +
                '```aerolang\nTimer.setTimeout(() => { Console.log("Done") }, 1000)\n```'
            ),
            'File': new vscode.MarkdownString(
                '**AeroLang I/O: File**\n\n' +
                'Object-oriented wrapper for File I/O operations.'
            ),
            'URL': new vscode.MarkdownString(
                '**AeroLang Network: URL**\n\n' +
                'URL parser and builder component.'
            ),
            'Path': new vscode.MarkdownString(
                '**AeroLang I/O: Path**\n\n' +
                'Route manipulation for file system paths.'
            ),
            'Stream': new vscode.MarkdownString(
                '**AeroLang I/O: Stream**\n\n' +
                'Memory-efficient Input and Output streams mapping.'
            ),
            'UUID': new vscode.MarkdownString(
                '**AeroLang Utility: UUID**\n\n' +
                'Generates v4 pseudo-random standard identifiers.'
            ),
            'Base64': new vscode.MarkdownString(
                '**AeroLang Encoding: Base64**\n\n' +
                'Base64 encoding and decoding utilities.'
            ),
            'Crypto': new vscode.MarkdownString(
                '**AeroLang Cryptography: Crypto**\n\n' +
                'Hashing functions like MD5 and SHA-256 wrapping Java MessageDigest.'
            ),
            'URLEncoding': new vscode.MarkdownString(
                '**AeroLang Encoding: URLEncoding**\n\n' +
                'Percent encode and decode standard web strings.'
            ),
            'View': new vscode.MarkdownString(
                '**AeroLang UI Component: View**\n\n' +
                'Base class for visual Android OS components.'
            ),
            'TextView': new vscode.MarkdownString(
                '**AeroLang UI Component: TextView**\n\n' +
                'A visual component that displays standard text.'
            ),
            'Activity': new vscode.MarkdownString(
                '**AeroLang Component: Activity**\n\n' +
                'A single, focused window that the user can interact with.'
            ),
            'Intent': new vscode.MarkdownString(
                '**AeroLang Component: Intent**\n\n' +
                'An abstract description of an operation to be performed.'
            ),
            'Camera': new vscode.MarkdownString(
                '**AeroLang Multimedia API: Camera**\n\n' +
                'Hardware interface to the device camera lenses.'
            ),
            'MediaPlayer': new vscode.MarkdownString(
                '**AeroLang Multimedia API: MediaPlayer**\n\n' +
                'Controls playback of audio/video files and streams natively.'
            ),
            'MediaRecorder': new vscode.MarkdownString(
                '**AeroLang Multimedia API: MediaRecorder**\n\n' +
                'Records audio and video internally.'
            ),
            'fun': new vscode.MarkdownString(
                '**AeroLang Keyword: fun**\n\n' +
                'Declares a function. Functions can optionally declare return types or simply evaluate into expressions.'
            ),
            'class': new vscode.MarkdownString(
                '**AeroLang Keyword: class**\n\n' +
                'Declares a custom type encapsulation mimicking C++ classes natively.'
            ),
            'var': new vscode.MarkdownString(
                '**AeroLang Keyword: var**\n\n' +
                'Declares a mutable variable.'
            ),
            'val': new vscode.MarkdownString(
                '**AeroLang Keyword: val**\n\n' +
                'Declares an immutable variable (constant).'
            ),
            // ── Collections ──────────────────────────────────────────────────
            'ArrayList': new vscode.MarkdownString(
                '**AeroLang Collection: ArrayList\<T\>**\n\n' +
                'A dynamically-sized ordered list backed by an `aero::ArrayList<T>` C++ template.\n' +
                '```aerolang\nval list = ArrayList<String>()\nlist.add("hello")\nval first = list.get(0)\n```'
            ),
            'HashMap': new vscode.MarkdownString(
                '**AeroLang Collection: HashMap\<K,V\>**\n\n' +
                'Key-value store with O(1) average-case operations backed by `aero::HashMap<K,V>`.\n' +
                '```aerolang\nval map = HashMap<String, Int>()\nmap.put("score", 100)\nval score = map.get("score")\n```'
            ),
            'HashSet': new vscode.MarkdownString(
                '**AeroLang Collection: HashSet\<T\>**\n\n' +
                'Unordered collection of unique elements. Supports set operations: union, intersection, difference.\n' +
                '```aerolang\nval tags = HashSet<String>()\ntags.add("kotlin")\ntags.add("android")\n```'
            ),
            // ── Networking ───────────────────────────────────────────────────
            'HttpClient': new vscode.MarkdownString(
                '**AeroLang Network: HttpClient**\n\n' +
                'Asynchronous HTTP client supporting GET, POST, PUT, DELETE.\n' +
                '```aerolang\nval client = HttpClient()\nclient.get("https://api.example.com/data", (response) => {\n    val body = response.getBody()\n})\n```'
            ),
            'WebSocket': new vscode.MarkdownString(
                '**AeroLang Network: WebSocket**\n\n' +
                'Full-duplex WebSocket client for real-time communication.\n' +
                '```aerolang\nval ws = WebSocket()\nws.connect("wss://echo.websocket.org")\nws.onMessage((msg) => { Console.log(msg) })\n```'
            ),
            'Response': new vscode.MarkdownString(
                '**AeroLang Network: Response**\n\n' +
                'Represents an HTTP response with status code, headers, and body.\n' +
                '```aerolang\nif (response.isSuccess()) {\n    val body = response.getBody()\n}\n```'
            ),
            // ── Data Persistence ─────────────────────────────────────────────
            'SharedPreferences': new vscode.MarkdownString(
                '**AeroLang Data: SharedPreferences**\n\n' +
                'Persistent key-value storage backed by Android SharedPreferences.\n' +
                '```aerolang\nval prefs = SharedPreferences("settings")\nprefs.putString("theme", "dark")\nprefs.commit()\n```'
            ),
            'Database': new vscode.MarkdownString(
                '**AeroLang Data: Database**\n\n' +
                'SQLite database wrapper with transaction support.\n' +
                '```aerolang\nval db = Database()\ndb.open("myapp.db")\ndb.exec("CREATE TABLE IF NOT EXISTS users (id INTEGER, name TEXT)")\n```'
            ),
            'JSON': new vscode.MarkdownString(
                '**AeroLang Data: JSON**\n\n' +
                'JSON parser and serializer.\n' +
                '```aerolang\nval json = JSON()\njson.parse(\'{"name":"AeroLang"}\')\nval name = json.get("name")\n```'
            ),
            'FileIO': new vscode.MarkdownString(
                '**AeroLang I/O: FileIO**\n\n' +
                'Reads and writes files on local storage.\n' +
                '```aerolang\nval io = FileIO()\nio.writeText("/data/note.txt", "Hello!")\nval text = io.readText("/data/note.txt")\n```'
            ),
            // ── Background ───────────────────────────────────────────────────
            'WorkManager': new vscode.MarkdownString(
                '**AeroLang Background: WorkManager**\n\n' +
                'Schedules and manages deferred background work that survives app restarts.\n' +
                '```aerolang\nval wm = WorkManager()\nwm.enqueue("SyncTask")\n```'
            ),
            'NotificationManager': new vscode.MarkdownString(
                '**AeroLang System: NotificationManager**\n\n' +
                'Posts and manages system notifications.\n' +
                '```aerolang\nval notif = NotificationManager()\nnotif.createChannel("updates", "App Updates", 3)\nnotif.show(1, "Hello", "You have a new message")\n```'
            ),
            // ── Navigation ───────────────────────────────────────────────────
            'Intent': new vscode.MarkdownString(
                '**AeroLang Component: Intent**\n\n' +
                'An abstract description of an operation — used to navigate between Activities.\n' +
                '```aerolang\nval intent = Intent(this, ProfileActivity::class)\nintent.putExtra("userId", "42")\nstartActivity(intent)\n```'
            ),
        };
    }

    provideHover(document, position, token) {
        const range = document.getWordRangeAtPosition(position);
        if (!range) {
            return undefined;
        }

        const word = document.getText(range);

        if (this.docs[word]) {
            return new vscode.Hover(this.docs[word]);
        }

        return undefined;
    }
}

module.exports = AeroLangHoverProvider;
