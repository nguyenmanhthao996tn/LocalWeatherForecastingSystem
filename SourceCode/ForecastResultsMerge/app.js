const resultFolderPath = 'KetQua/';

var fs = require('fs');

fs.readdir(resultFolderPath, function (err, items) {
    items.forEach(filename => {
        fs.readFile(resultFolderPath + filename, 'utf8', function (err, data) {
            if (err) throw err;

            var recordArray = data.split('\n');
            recordArray.forEach(element => {
                var recordElements = element.split('  ');

                if (recordElements != '') {
                    if (recordElements.length == 3) {
                        var dateString = '2018-' + filename.substring(0, 2) + '-' + filename.substring(2, 4);

                        var str = dateString + '\t' + recordElements[0] + '\t' + recordElements[1] + '\t' + recordElements[2] + '\n';
                        fs.appendFile('output.txt', str, function (err) {
                            if (err) throw err;
                            console.log('Saved!');
                          });
                    } else {
                        console.log("Length: " + recordElements.length);
                    }
                }
            });
        });
    });
});