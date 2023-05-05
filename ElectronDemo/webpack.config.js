const webpack = require('webpack')
const path = require('path')
const HtmlWebpackPlugin = require('html-webpack-plugin')
const MiniCssExtractPlugin = require('mini-css-extract-plugin')
const { spawn } = require('child_process')

// Any directories you will be adding code/files into, need to be added to this array so webpack will pick them up
const appPath = "./src/renderer"
const defaultInclude = path.resolve(__dirname, appPath)

const pluginsDev = [
  new HtmlWebpackPlugin(),
  new webpack.DefinePlugin({
    'process.env.NODE_ENV': JSON.stringify('development')
  })
];

const plugins = [
  new HtmlWebpackPlugin(),
  new MiniCssExtractPlugin({
    // Options similar to the same options in webpackOptions.output
    // both options are optional
    filename: 'bundle.css',
    chunkFilename: '[id].css'
  }),
  new webpack.DefinePlugin({
    'process.env.NODE_ENV': JSON.stringify('production')
  }),
];

module.exports = (mode) => {
  console.log(mode);
  const isProduction = !(mode.WEBPACK_SERVE == true);
  console.log('Env: '+ (isProduction ? 'production' : 'development'));
  return {
    module: {
      rules: [
        {
          test: /\.css$/,
          use: [{ loader: 'style-loader' }, { loader: 'css-loader' }, { loader: 'postcss-loader' }],
          include: defaultInclude
        },
        {
          test: /\.jsx?$/,
          use: [{ loader: 'babel-loader' }],
          include: defaultInclude
        },
        {
          test: /\.(jpe?g|png|gif)$/,
          use: [{ loader: 'file-loader?name=img/[name]__[hash:base64:5].[ext]' }],
          include: defaultInclude
        },
        {
          test: /\.(eot|svg|ttf|woff|woff2)$/,
          use: [{ loader: 'file-loader?name=font/[name]__[hash:base64:5].[ext]' }],
          include: defaultInclude
        }
      ]
    },
    target: 'electron-renderer',
    entry: appPath + '/index',
    // output: {
    //   path:path.join(__dirname,'./out/'),
    //   filename:'main.js'
    // },
    plugins: isProduction ? plugins : pluginsDev,
    devtool: isProduction ? 'source-map':'inline-source-map',
    devServer: isProduction ? {} : {
      port: 8080,
      compress: true,
      hot: true,
      headers: { 'Access-Control-Allow-Origin': '*' },
      static: {
        publicPath: '/',
      },
      historyApiFallback: {
        verbose: true,
      },
      setupMiddlewares(middlewares) {
        spawn(
          'electron',
          ['.'],
          { shell: true, env: process.env, stdio: 'inherit' }
        )
        .on('close', code => process.exit(0))
        .on('error', spawnError => console.error(spawnError));
        console.log('Starting Renderer Process...');
        return middlewares;
      },
    },

    stats: isProduction ? {
      colors: true,
      children: false,
      chunks: false
    } : {},
    optimization: isProduction ? {
      minimize: true
    } : {}
  }
}
