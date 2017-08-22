'use babel';

import pGDSView from './pGDS-view';
import { CompositeDisposable } from 'atom';

export default {

  pGDSView: null,
  modalPanel: null,
  subscriptions: null,
  win: null,
  pane: null,

  activate(state) {
    this.pGDSView = new pGDSView(state.pGDSViewState);
    this.modalPanel = atom.workspace.addModalPanel({
      item: this.pGDSView.getElement(),
      visible: false
    });

    // this.pane = atom.workspace.panelForItem(this)
    // this.pane.hide();

    // In the main process.
    // const {BrowserWindow} = require('electron')

    // Or use `remote` from the renderer process.
    // const {BrowserWindow} = require('electron').remote
    // // titleBarStyle: 'hiddenInset',
    //
    // // var trans = 0x00000000;, backgroundColor: trans
    //
    // this.win = new BrowserWindow({width: 800, height: 600, show: false, vibrancy: 'light', title: '.gds Maker?'}) //, transparent: true,
    // this.win.on('closed', () => {
    //   this.win = null
    // })
    // let win = new BrowserWindow({show: false})
    // this.win.once('ready-to-show', () => {
    //   this.win.show();
    // })

    // this.win.show();

    // require('context.cc').test(this.win.getNativeWindowHandle())

    // console.log('Tried adding!');
    // // try {
    var addon = require('../pGDS/build/Release/pGDS');
    // // } catch (e) {
    // //     console.log(e.name + e.message);
    // // }
    // console.log('Finished adding!');
    //
    // // const addon = require('./build/Release/addon');
    //
    // var win = this.win.getNativeWindowHandle();
    // console.log(win.toString('hex'));

    const obj = new addon.WINDOW(0);


    // const obj = new addon.WINDOW(10);
    // console.log(obj.plusOne());
    // // Prints: 11
    // console.log(obj.plusOne());
    // // Prints: 12
    // console.log(obj.plusOne());
    // // Prints: 13

    // var test  = new addon.WINDOW(this.win.getNativeWindowHandle());

    // Load a remote URL
    // this.win.loadURL('https://github.com')

    // win.setRepresentedFilename('/etc/passwd')
    // win.setDocumentEdited(true)


    // Events subscribed to in atom's system can be easily cleaned up with a CompositeDisposable
    this.subscriptions = new CompositeDisposable();

    // Register command that toggles this view
    this.subscriptions.add(atom.commands.add('atom-workspace', {
      'gds-test:toggle': () => this.toggle()
    }));
  },

  deactivate() {
    this.modalPanel.destroy();
    this.subscriptions.dispose();
    this.pGDSView.destroy();
    this.pane.destroy();
  },

  serialize() {
    return {
      pGDSViewState: this.pGDSView.serialize()
    };
  },

  toggle() {
    console.log('pGDS was toggled!');
    // // this.win.flashFrame(true);
    // return (
    // //   this.pane.isVisible() ?
    // //   this.pane.hide() :
    // //   this.pane.show()
    //     // this.win.isVisible() ?
    //     // this.win.hide() :
    //     // this.win.show()
    // );
  }

};
