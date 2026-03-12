const vscode = require('vscode');

// ─────────────────────────────────────────────────────────────────────────────
//  AeroLang IntelliSense Provider
//  Provides: keyword completions, type completions, method dot-completions,
//            generic-aware snippets, and document-scanned variable tracking.
// ─────────────────────────────────────────────────────────────────────────────

const KEYWORDS = [
    'activity', 'fragment', 'service', 'class', 'fun', 'var', 'val',
    'override', 'private', 'public', 'protected', 'return', 'if', 'else',
    'when', 'for', 'while', 'break', 'continue', 'true', 'false', 'null',
    'this', 'super', 'in', 'is', 'as',
];

const PRIMITIVE_TYPES = [
    'String', 'Int', 'Long', 'Short', 'Byte', 'Double', 'Float', 'Boolean',
    'Char', 'Unit', 'Any',
];

const COLLECTION_TYPES = [
    'ArrayList', 'HashMap', 'HashSet', 'List', 'Map', 'Set',
];

const RUNTIME_TYPES = [
    // Android components
    'Activity', 'Fragment', 'Service', 'Intent', 'BroadcastReceiver',
    // Data
    'SharedPreferences', 'Database', 'JSON', 'FileIO', 'File', 'Stream',
    // Network
    'HttpClient', 'WebSocket', 'URL', 'URLEncoding', 'Response',
    // Background
    'WorkManager', 'AsyncTask', 'Timer',
    // Notifications
    'NotificationManager',
    // Media
    'Camera', 'MediaPlayer', 'MediaRecorder', 'VideoView',
    // Hardware
    'LocationManager', 'SensorManager',
    // Utilities
    'Log', 'Console', 'Math', 'DateTime', 'UUID', 'Base64', 'Crypto',
    'Regex', 'Path', 'JNICache',
];

// Per-type method completions  { typeName: [ { name, params, returns, detail } ] }
const TYPE_METHODS = {
    'ArrayList': [
        { name: 'add', params: 'item: T', returns: 'Unit', detail: 'Append an element to the list' },
        { name: 'get', params: 'index: Int', returns: 'T', detail: 'Get element at index' },
        { name: 'remove', params: 'index: Int', returns: 'Unit', detail: 'Remove element at index' },
        { name: 'contains', params: 'item: T', returns: 'Boolean', detail: 'Check if element exists' },
        { name: 'indexOf', params: 'item: T', returns: 'Int', detail: 'Find first index of element' },
        { name: 'size', params: '', returns: 'Int', detail: 'Number of elements' },
        { name: 'clear', params: '', returns: 'Unit', detail: 'Remove all elements' },
        { name: 'isEmpty', params: '', returns: 'Boolean', detail: 'Check if list is empty' },
        { name: 'forEach', params: 'action: (T) => Unit', returns: 'Unit', detail: 'Iterate over each element' },
    ],
    'HashMap': [
        { name: 'put', params: 'key: K, value: V', returns: 'Unit', detail: 'Insert or update a key-value pair' },
        { name: 'get', params: 'key: K', returns: 'V', detail: 'Get value for key' },
        { name: 'remove', params: 'key: K', returns: 'Unit', detail: 'Remove entry by key' },
        { name: 'containsKey', params: 'key: K', returns: 'Boolean', detail: 'Check if key exists' },
        { name: 'containsValue', params: 'value: V', returns: 'Boolean', detail: 'Check if value exists' },
        { name: 'keys', params: '', returns: 'ArrayList<K>', detail: 'Get all keys' },
        { name: 'values', params: '', returns: 'ArrayList<V>', detail: 'Get all values' },
        { name: 'size', params: '', returns: 'Int', detail: 'Number of entries' },
        { name: 'clear', params: '', returns: 'Unit', detail: 'Remove all entries' },
        { name: 'getOrDefault', params: 'key: K, default: V', returns: 'V', detail: 'Get value or default if key absent' },
    ],
    'HashSet': [
        { name: 'add', params: 'item: T', returns: 'Unit', detail: 'Add unique element' },
        { name: 'remove', params: 'item: T', returns: 'Unit', detail: 'Remove element' },
        { name: 'contains', params: 'item: T', returns: 'Boolean', detail: 'Check membership' },
        { name: 'size', params: '', returns: 'Int', detail: 'Number of elements' },
        { name: 'clear', params: '', returns: 'Unit', detail: 'Remove all elements' },
        { name: 'union', params: 'other: HashSet<T>', returns: 'HashSet<T>', detail: 'Union of two sets' },
        { name: 'intersection', params: 'other: HashSet<T>', returns: 'HashSet<T>', detail: 'Intersection of two sets' },
        { name: 'difference', params: 'other: HashSet<T>', returns: 'HashSet<T>', detail: 'Difference of two sets' },
        { name: 'isSubsetOf', params: 'other: HashSet<T>', returns: 'Boolean', detail: 'Check if subset' },
        { name: 'toArrayList', params: '', returns: 'ArrayList<T>', detail: 'Convert to ArrayList' },
    ],
    'HttpClient': [
        { name: 'get', params: 'url: String, callback: (Response) => Unit', returns: 'Unit', detail: 'HTTP GET request' },
        { name: 'post', params: 'url: String, body: String, callback: (Response) => Unit', returns: 'Unit', detail: 'HTTP POST request' },
        { name: 'put', params: 'url: String, body: String, callback: (Response) => Unit', returns: 'Unit', detail: 'HTTP PUT request' },
        { name: 'delete', params: 'url: String, callback: (Response) => Unit', returns: 'Unit', detail: 'HTTP DELETE request' },
        { name: 'setHeader', params: 'key: String, value: String', returns: 'Unit', detail: 'Set a request header' },
        { name: 'setTimeout', params: 'ms: Int', returns: 'Unit', detail: 'Set request timeout in milliseconds' },
    ],
    'WebSocket': [
        { name: 'connect', params: 'url: String', returns: 'Unit', detail: 'Connect to a WebSocket server' },
        { name: 'send', params: 'message: String', returns: 'Unit', detail: 'Send a message' },
        { name: 'close', params: '', returns: 'Unit', detail: 'Close the connection' },
        { name: 'onMessage', params: 'callback: (String) => Unit', returns: 'Unit', detail: 'Handle incoming messages' },
        { name: 'onOpen', params: 'callback: () => Unit', returns: 'Unit', detail: 'Handle connection open' },
        { name: 'onClose', params: 'callback: () => Unit', returns: 'Unit', detail: 'Handle connection close' },
        { name: 'isConnected', params: '', returns: 'Boolean', detail: 'Check if connected' },
    ],
    'SharedPreferences': [
        { name: 'getString', params: 'key: String, default: String', returns: 'String', detail: 'Get a string preference' },
        { name: 'getInt', params: 'key: String, default: Int', returns: 'Int', detail: 'Get an int preference' },
        { name: 'getBoolean', params: 'key: String, default: Boolean', returns: 'Boolean', detail: 'Get a boolean preference' },
        { name: 'putString', params: 'key: String, value: String', returns: 'Unit', detail: 'Save a string preference' },
        { name: 'putInt', params: 'key: String, value: Int', returns: 'Unit', detail: 'Save an int preference' },
        { name: 'putBoolean', params: 'key: String, value: Boolean', returns: 'Unit', detail: 'Save a boolean preference' },
        { name: 'remove', params: 'key: String', returns: 'Unit', detail: 'Remove a preference entry' },
        { name: 'commit', params: '', returns: 'Unit', detail: 'Commit changes to disk' },
        { name: 'clear', params: '', returns: 'Unit', detail: 'Clear all preferences' },
    ],
    'Database': [
        { name: 'open', params: 'name: String', returns: 'Unit', detail: 'Open or create a database' },
        { name: 'exec', params: 'sql: String', returns: 'Unit', detail: 'Execute raw SQL' },
        { name: 'query', params: 'sql: String', returns: 'String', detail: 'Execute a SELECT query' },
        { name: 'insert', params: 'table: String, values: HashMap<String, String>', returns: 'Long', detail: 'Insert a row' },
        { name: 'update', params: 'table: String, values: HashMap<String, String>, where: String', returns: 'Int', detail: 'Update rows' },
        { name: 'delete', params: 'table: String, where: String', returns: 'Int', detail: 'Delete rows' },
        { name: 'close', params: '', returns: 'Unit', detail: 'Close the database' },
        { name: 'beginTransaction', params: '', returns: 'Unit', detail: 'Begin a transaction' },
        { name: 'commit', params: '', returns: 'Unit', detail: 'Commit the transaction' },
        { name: 'rollback', params: '', returns: 'Unit', detail: 'Rollback the transaction' },
    ],
    'JSON': [
        { name: 'parse', params: 'jsonString: String', returns: 'JSON', detail: 'Parse a JSON string' },
        { name: 'stringify', params: 'indent: Int', returns: 'String', detail: 'Serialize to JSON string' },
        { name: 'get', params: 'key: String', returns: 'String', detail: 'Get value by key' },
        { name: 'getInt', params: 'key: String', returns: 'Int', detail: 'Get integer value by key' },
        { name: 'getDouble', params: 'key: String', returns: 'Double', detail: 'Get double value by key' },
        { name: 'getBoolean', params: 'key: String', returns: 'Boolean', detail: 'Get boolean value by key' },
        { name: 'getArray', params: 'key: String', returns: 'ArrayList<JSON>', detail: 'Get nested array' },
        { name: 'set', params: 'key: String, value: String', returns: 'Unit', detail: 'Set a key-value pair' },
        { name: 'has', params: 'key: String', returns: 'Boolean', detail: 'Check if key exists' },
    ],
    'FileIO': [
        { name: 'readText', params: 'path: String', returns: 'String', detail: 'Read entire file as text' },
        { name: 'writeText', params: 'path: String, content: String', returns: 'Unit', detail: 'Write text to file' },
        { name: 'appendText', params: 'path: String, content: String', returns: 'Unit', detail: 'Append text to file' },
        { name: 'readLines', params: 'path: String', returns: 'ArrayList<String>', detail: 'Read file as lines' },
        { name: 'exists', params: 'path: String', returns: 'Boolean', detail: 'Check if file exists' },
        { name: 'delete', params: 'path: String', returns: 'Unit', detail: 'Delete a file' },
        { name: 'listFiles', params: 'dir: String', returns: 'ArrayList<String>', detail: 'List files in directory' },
        { name: 'mkdir', params: 'path: String', returns: 'Unit', detail: 'Create a directory' },
        { name: 'copyFile', params: 'src: String, dst: String', returns: 'Unit', detail: 'Copy a file' },
    ],
    'NotificationManager': [
        { name: 'createChannel', params: 'id: String, name: String, importance: Int', returns: 'Unit', detail: 'Create a notification channel (Android 8+)' },
        { name: 'show', params: 'id: Int, title: String, body: String', returns: 'Unit', detail: 'Show a simple notification' },
        { name: 'showWithAction', params: 'id: Int, title: String, body: String, action: String', returns: 'Unit', detail: 'Notification with tap action' },
        { name: 'cancel', params: 'id: Int', returns: 'Unit', detail: 'Cancel a notification' },
        { name: 'cancelAll', params: '', returns: 'Unit', detail: 'Cancel all notifications' },
    ],
    'WorkManager': [
        { name: 'enqueue', params: 'work: String', returns: 'Unit', detail: 'Schedule a deferred background task' },
        { name: 'cancel', params: 'id: String', returns: 'Unit', detail: 'Cancel a previously enqueued task' },
        { name: 'cancelAll', params: '', returns: 'Unit', detail: 'Cancel all pending tasks' },
    ],
    'Timer': [
        { name: 'setTimeout', params: 'callback: () => Unit, ms: Int', returns: 'Unit', detail: 'Run callback after a delay' },
        { name: 'setInterval', params: 'callback: () => Unit, ms: Int', returns: 'Unit', detail: 'Run callback repeatedly at interval' },
        { name: 'clearTimeout', params: 'id: Int', returns: 'Unit', detail: 'Cancel a timeout' },
        { name: 'clearInterval', params: 'id: Int', returns: 'Unit', detail: 'Cancel an interval' },
    ],
    'Log': [
        { name: 'd', params: 'tag: String, msg: String', returns: 'Unit', detail: 'Debug log to Logcat' },
        { name: 'i', params: 'tag: String, msg: String', returns: 'Unit', detail: 'Info log to Logcat' },
        { name: 'w', params: 'tag: String, msg: String', returns: 'Unit', detail: 'Warning log to Logcat' },
        { name: 'e', params: 'tag: String, msg: String', returns: 'Unit', detail: 'Error log to Logcat' },
    ],
    'Console': [
        { name: 'log', params: 'message: String', returns: 'Unit', detail: 'Print a log message' },
        { name: 'error', params: 'message: String', returns: 'Unit', detail: 'Print an error message' },
        { name: 'warn', params: 'message: String', returns: 'Unit', detail: 'Print a warning message' },
        { name: 'time', params: 'label: String', returns: 'Unit', detail: 'Start a performance timer' },
        { name: 'timeEnd', params: 'label: String', returns: 'Unit', detail: 'End a performance timer and print elapsed' },
    ],
    'Camera': [
        { name: 'open', params: 'facing: Int', returns: 'Unit', detail: 'Open camera (0=back, 1=front)' },
        { name: 'startPreview', params: '', returns: 'Unit', detail: 'Begin camera preview' },
        { name: 'stopPreview', params: '', returns: 'Unit', detail: 'Stop camera preview' },
        { name: 'takePicture', params: 'callback: (String) => Unit', returns: 'Unit', detail: 'Capture a photo, callback receives file path' },
        { name: 'release', params: '', returns: 'Unit', detail: 'Release camera resources' },
    ],
    'MediaPlayer': [
        { name: 'setDataSource', params: 'path: String', returns: 'Unit', detail: 'Set path to audio/video file' },
        { name: 'prepare', params: '', returns: 'Unit', detail: 'Prepare synchronously' },
        { name: 'prepareAsync', params: '', returns: 'Unit', detail: 'Prepare asynchronously' },
        { name: 'start', params: '', returns: 'Unit', detail: 'Start playback' },
        { name: 'pause', params: '', returns: 'Unit', detail: 'Pause playback' },
        { name: 'stop', params: '', returns: 'Unit', detail: 'Stop playback' },
        { name: 'release', params: '', returns: 'Unit', detail: 'Release resources' },
        { name: 'isPlaying', params: '', returns: 'Boolean', detail: 'Check if playing' },
        { name: 'seekTo', params: 'msec: Int', returns: 'Unit', detail: 'Seek to position in milliseconds' },
        { name: 'getDuration', params: '', returns: 'Int', detail: 'Get duration in milliseconds' },
    ],
    'LocationManager': [
        { name: 'getLatitude', params: '', returns: 'Double', detail: 'Last known latitude' },
        { name: 'getLongitude', params: '', returns: 'Double', detail: 'Last known longitude' },
        { name: 'requestLocationUpdates', params: 'callback: (Double, Double) => Unit', returns: 'Unit', detail: 'Start GPS polling' },
        { name: 'stopLocationUpdates', params: '', returns: 'Unit', detail: 'Stop GPS polling' },
        { name: 'hasPermission', params: '', returns: 'Boolean', detail: 'Check if location permission is granted' },
    ],
    'SensorManager': [
        { name: 'getAccelerometerData', params: '', returns: 'String', detail: 'XYZ accelerometer vector' },
        { name: 'getGyroscopeData', params: '', returns: 'String', detail: 'XYZ gyroscope vector' },
        { name: 'getCompassData', params: '', returns: 'String', detail: 'XYZ magnetometer vector' },
        { name: 'getProximityData', params: '', returns: 'Double', detail: 'Proximity distance in cm' },
        { name: 'startListening', params: '', returns: 'Unit', detail: 'Begin sensor observation' },
        { name: 'stopListening', params: '', returns: 'Unit', detail: 'Stop sensor observation' },
    ],
    'Intent': [
        { name: 'putExtra', params: 'key: String, value: String', returns: 'Unit', detail: 'Attach a string extra' },
        { name: 'getExtra', params: 'key: String', returns: 'String', detail: 'Retrieve a string extra' },
        { name: 'setAction', params: 'action: String', returns: 'Unit', detail: 'Set the intent action' },
        { name: 'addFlag', params: 'flag: Int', returns: 'Unit', detail: 'Add an intent flag' },
    ],
    'Response': [
        { name: 'getBody', params: '', returns: 'String', detail: 'Response body as string' },
        { name: 'getStatusCode', params: '', returns: 'Int', detail: 'HTTP status code' },
        { name: 'getHeader', params: 'key: String', returns: 'String', detail: 'Get a response header value' },
        { name: 'isSuccess', params: '', returns: 'Boolean', detail: 'True if status 2xx' },
    ],
};

// ─────────────────────────────────────────────────────────────────────────────
//  Snippets
// ─────────────────────────────────────────────────────────────────────────────
const SNIPPETS = [
    {
        label: 'activity',
        detail: 'Declare an Activity',
        insert: 'activity ${1:MainActivity} {\n\toverride fun onCreate() {\n\t\t$0\n\t}\n}',
    },
    {
        label: 'fragment',
        detail: 'Declare a Fragment',
        insert: 'fragment ${1:MyFragment} {\n\toverride fun onCreate() {\n\t\t$0\n\t}\n}',
    },
    {
        label: 'service',
        detail: 'Declare a Service',
        insert: 'service ${1:MyService} {\n\toverride fun onCreate() {\n\t\t$0\n\t}\n\toverride fun onDestroy() {\n\t}\n}',
    },
    {
        label: 'class',
        detail: 'Declare a class',
        insert: 'class ${1:MyClass} {\n\t$0\n}',
    },
    {
        label: 'fun',
        detail: 'Function declaration',
        insert: 'fun ${1:name}(${2:param}: ${3:Type}): ${4:Unit} {\n\t$0\n}',
    },
    {
        label: 'override fun onCreate',
        detail: 'Android lifecycle: onCreate',
        insert: 'override fun onCreate() {\n\t$0\n}',
    },
    {
        label: 'override fun onResume',
        detail: 'Android lifecycle: onResume',
        insert: 'override fun onResume() {\n\t$0\n}',
    },
    {
        label: 'override fun onPause',
        detail: 'Android lifecycle: onPause',
        insert: 'override fun onPause() {\n\t$0\n}',
    },
    {
        label: 'override fun onDestroy',
        detail: 'Android lifecycle: onDestroy',
        insert: 'override fun onDestroy() {\n\t$0\n}',
    },
    {
        label: 'for range',
        detail: 'For-range loop',
        insert: 'for (${1:i} in ${2:0}..${3:10}) {\n\t$0\n}',
    },
    {
        label: 'if/else',
        detail: 'If-else block',
        insert: 'if (${1:condition}) {\n\t$0\n} else {\n\t\n}',
    },
    {
        label: 'when',
        detail: 'When expression (switch)',
        insert: 'when (${1:value}) {\n\t${2:case1} -> { $0 }\n\telse -> { }\n}',
    },
    {
        label: 'ArrayList<T>',
        detail: 'Create an ArrayList',
        insert: 'ArrayList<${1:String}>()',
    },
    {
        label: 'HashMap<K,V>',
        detail: 'Create a HashMap',
        insert: 'HashMap<${1:String}, ${2:Int}>()',
    },
    {
        label: 'HashSet<T>',
        detail: 'Create a HashSet',
        insert: 'HashSet<${1:String}>()',
    },
    {
        label: 'HttpClient GET',
        detail: 'HTTP GET request',
        insert: 'val ${1:client} = HttpClient()\n${1:client}.get("${2:https://api.example.com}", (response) => {\n\tval body = response.getBody()\n\t$0\n})',
    },
    {
        label: 'SharedPreferences',
        detail: 'Open SharedPreferences',
        insert: 'val ${1:prefs} = SharedPreferences("${2:settings}")\n${1:prefs}.putString("${3:key}", "${4:value}")\n${1:prefs}.commit()',
    },
    {
        label: 'Intent navigate',
        detail: 'Navigate to another Activity',
        insert: 'val ${1:intent} = Intent(this, ${2:TargetActivity}::class)\n${1:intent}.putExtra("${3:key}", "${4:value}")\nstartActivity(${1:intent})',
    },
    {
        label: 'Log.d',
        detail: 'Debug log',
        insert: 'Log.d("${1:TAG}", "${2:message}")',
    },
    {
        label: 'val var declaration',
        detail: 'Declare a variable',
        insert: '${1|val,var|} ${2:name}: ${3:String} = ${4:"value"}',
    },
];

// ─────────────────────────────────────────────────────────────────────────────
//  Variable Tracker — scan document for  var/val <name>: <Type>
// ─────────────────────────────────────────────────────────────────────────────
function scanDocumentVariables(document) {
    /** @type {Map<string, string>} varName -> typeName */
    const vars = new Map();
    const varPattern = /\b(?:var|val)\s+(\w+)\s*:\s*(\w+)/g;
    const fullText = document.getText();
    let m;
    while ((m = varPattern.exec(fullText)) !== null) {
        vars.set(m[1], m[2]);
    }
    return vars;
}

// ─────────────────────────────────────────────────────────────────────────────
//  Provider Class
// ─────────────────────────────────────────────────────────────────────────────
class AeroLangIntelliSenseProvider {

    provideCompletionItems(document, position /*, token, context */) {
        const linePrefix = document.lineAt(position).text.substring(0, position.character);

        // ── Dot completion: someVar.<cursor> ─────────────────────────────────
        if (linePrefix.endsWith('.')) {
            const tokenBefore = linePrefix.slice(0, -1).match(/(\w+)$/)?.[1] || '';

            // 1. Check if last token is a known type name directly
            let resolvedType = TYPE_METHODS[tokenBefore] ? tokenBefore : null;

            // 2. Fall back to document variable scan
            if (!resolvedType) {
                const vars = scanDocumentVariables(document);
                const varType = vars.get(tokenBefore);
                if (varType && TYPE_METHODS[varType]) {
                    resolvedType = varType;
                }
            }

            if (resolvedType) {
                return TYPE_METHODS[resolvedType].map(m => {
                    const item = new vscode.CompletionItem(m.name, vscode.CompletionItemKind.Method);
                    item.detail = `(${m.params}): ${m.returns}`;
                    item.documentation = new vscode.MarkdownString(
                        `**${resolvedType}.${m.name}**\n\n${m.detail}`
                    );
                    // Insert with parens, put cursor inside
                    item.insertText = new vscode.SnippetString(
                        m.params ? `${m.name}($0)` : `${m.name}()`
                    );
                    item.command = m.params
                        ? { command: 'editor.action.triggerParameterHints', title: 'Trigger param hints' }
                        : undefined;
                    return item;
                });
            }

            return [];
        }

        // ── Top-level completions ─────────────────────────────────────────────
        const items = [];

        // Keywords
        for (const kw of KEYWORDS) {
            const item = new vscode.CompletionItem(kw, vscode.CompletionItemKind.Keyword);
            items.push(item);
        }

        // Primitive types
        for (const t of PRIMITIVE_TYPES) {
            const item = new vscode.CompletionItem(t, vscode.CompletionItemKind.Class);
            item.detail = 'AeroLang primitive type';
            items.push(item);
        }

        // Collection types
        for (const t of COLLECTION_TYPES) {
            const item = new vscode.CompletionItem(t, vscode.CompletionItemKind.Class);
            item.detail = 'AeroLang collection type';
            items.push(item);
        }

        // Runtime types
        for (const t of RUNTIME_TYPES) {
            const item = new vscode.CompletionItem(t, vscode.CompletionItemKind.Class);
            item.detail = 'AeroLang runtime type';
            items.push(item);
        }

        // Snippets
        for (const s of SNIPPETS) {
            const item = new vscode.CompletionItem(s.label, vscode.CompletionItemKind.Snippet);
            item.detail = s.detail;
            item.insertText = new vscode.SnippetString(s.insert);
            item.sortText = `0_${s.label}`; // show snippets first
            items.push(item);
        }

        return items;
    }
}

module.exports = AeroLangIntelliSenseProvider;
