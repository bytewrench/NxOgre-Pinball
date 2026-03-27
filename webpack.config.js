const path = require('path');
const HtmlWebpackPlugin = require('html-webpack-plugin');

module.exports = {
    entry: './src/index.js',
    output: {
        path: path.resolve(__dirname, 'dist'),
        filename: 'bundle.js',
    },
    module: {
        rules: [
            {
                test: /.js$/,  // Transpile JavaScript files
                exclude: /node_modules/,  
                use: {
                    loader: 'babel-loader',
                },
            },
            {
                test: /.json$/,  // Allow importing of JSON files
                type: 'javascript/auto',
                use: 'json-loader',
            },
        ],
    },
    plugins: [
        new HtmlWebpackPlugin({
            template: './src/index.html',
        }),
    ],
    resolve: {
        extensions: ['.js', '.json'],
        alias: {
            three: path.resolve(__dirname, 'node_modules/three'),
            'cannon-es': path.resolve(__dirname, 'node_modules/cannon-es'),
        },
    },
    devServer: {
        contentBase: path.join(__dirname, 'dist'),
        compress: true,
        port: 9000,
    },
};